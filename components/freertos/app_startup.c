/*
 * SPDX-FileCopyrightText: 2022-2026 Espressif Systems (Shanghai) CO LTD
 *
 * SPDX-License-Identifier: Apache-2.0
 */

#ਅਨ੍ਤਰ੍ਭਾਵ "sdkconfig.h"
#ਅਨ੍ਤਰ੍ਭਾਵ <stddef.h>
#ਅਨ੍ਤਰ੍ਭਾਵ <assert.h>
#ਅਨ੍ਤਰ੍ਭਾਵ "esp_task.h"
#ਅਨ੍ਤਰ੍ਭਾਵ "esp_log.h"
#ਅਨ੍ਤਰ੍ਭਾਵ "freertos/FreeRTOS.h"
#ਅਨ੍ਤਰ੍ਭਾਵ "freertos/task.h"
#ਅਨ੍ਤਰ੍ਭਾਵ "freertos/portmacro.h"
#ਅਨ੍ਤਰ੍ਭਾਵ "esp_private/esp_sys_event_internal.h"

/* ------------------------------------------------- App/OS Startup ----------------------------------------------------
 * - Functions related to application and FreeRTOS startup
 * - This startup is common to all architectures (e.g. RISC-V and Xtensa) and all FreeRTOS implementations (i.e., IDF
 *   FreeRTOS and Amazon SMP FreeRTOS).
 * - Application startup flow as follows:
 *      - For CPU 0
 *          - CPU0 completes CPU startup (in startup.c), then calls esp_startup_start_app()
 *          - esp_startup_start_app() registers some daemon services for CPU0 then starts FreeRTOS
 *      - For CPUx (1 to N-1)
 *          - CPUx completes CPU startup in startup.c, then calls esp_startup_start_app_other_cores()
 *          - esp_startup_start_app_other_cores(), registers some daemon services for CPUx, waits for CPU0 to start
 *            FreeRTOS, then yields (via xPortStartScheduler()) to schedule a task.
 * ------------------------------------------------------------------------------------------------------------------ */

// ----------------------- Checks --------------------------

/*
For now, AMP is not supported (i.e., running FreeRTOS on one core and a bare metal/other OS on the other). Therefore,
CONFIG_FREERTOS_UNICORE and CONFIG_ESP_SYSTEM_SINGLE_CORE_MODE should be identical. We add a check for this here.
*/
#ਜੇ CONFIG_FREERTOS_UNICORE != CONFIG_ESP_SYSTEM_SINGLE_CORE_ਬਹੁਲਕ
#ਗਲਤੀ "AMP not supported. FreeRTOS number of cores and system number of cores must be identical"
#ਜੇ_ਅੰਤ

// -------------------- Declarations -----------------------

ਸਥਿਰ ਖਾਲੀ main_task(ਖਾਲੀ* ਦਲੀਲਾਂ);
ESP_LOG_ATTR_ਟੈਗ(APP_ਸ਼ੁਰੂ_ਟੈਗ, "app_start");

ਸਥਿਰ ਖਾਲੀ ਲੜੀਵਾਰ_app_startup_event(esp_sys_event_id_t id)
{
    ESP_SYS_EVENT_FOREACH(handler, id) {
        ESP_ERROR_CHECK(handler->handler(ਨਿਰਾ, ਨਿਰਾ));
    }
}

// ------------------ CPU0 App Startup ---------------------

ਖਾਲੀ esp_startup_ਸ਼ੁਰੂ_app(ਖਾਲੀ)
{
    ਲੜੀਵਾਰ_app_startup_event(ESP_SYS_EVENT_PRE_SCHEDULER);

    ਅਧਾਰ_Type_t res = x_Task_Create_Pinned_ਮੰਜ਼ਿਲ_Core(main_task, "main",
                                             ESP_TASK_MAIN_ਢੇਰ, ਨਿਰਾ,
                                             ESP_TASK_MAIN_PRIO, ਨਿਰਾ, ESP_TASK_MAIN_CORE);
    ਪ੍ਰਤਿਜ੍ਞਾ(res == pdTRUE);
    (ਖਾਲੀ)res;

    /*
    If a particular FreeRTOS port has port/arch specific OS startup behavior, they can implement a function of type
    "void port_start_app_hook(void)" in their `port.c` files. This function will be called below, thus allowing each
    FreeRTOS port to implement port specific app startup behavior.

    Deprecated: Register a PRE_SCHEDULER handler with priority 999 to preserve this ordering.
    */
    // TODO: IDF-16135
    ਖਾਲੀ __attribute__((weak, deprecated("port_start_app_hook is deprecated and will be removed in IDF 7.0. Use ESP_PRE_SCHEDULER_HANDLER_REGISTER instead.")))
    ਪੋਰਟ_ਸ਼ੁਰੂ_app_hook(ਖਾਲੀ);
#ਹਦਾਇਤ GCC diagnostic ਧੱਕੋ
#ਹਦਾਇਤ GCC diagnostic ignored "-Wdeprecated-declarations"
    ਜੇ (ਪੋਰਟ_ਸ਼ੁਰੂ_app_hook != ਨਿਰਾ) {
        ESP_EARLY_LOGW(APP_ਸ਼ੁਰੂ_ਟੈਗ,
                       "port_start_app_hook is deprecated and will be removed in IDF 7.0. "
                       "Use ESP_PRE_SCHEDULER_HANDLER_REGISTER instead.");
        ਪੋਰਟ_ਸ਼ੁਰੂ_app_hook();
    }
#ਹਦਾਇਤ GCC diagnostic ਕੱਢੋ

    ESP_EARLY_LOGD(APP_ਸ਼ੁਰੂ_ਟੈਗ, "Starting scheduler on CPU0");
    v_Task_ਸ਼ੁਰੂ_Scheduler();
}

// --------------- CPU[1:N-1] App Startup ------------------

#ਜੇ !CONFIG_FREERTOS_UNICORE
ਖਾਲੀ esp_startup_ਸ਼ੁਰੂ_app_other_cores(ਖਾਲੀ)
{
    // For now, we only support up to two core: 0 and 1.
    ਜੇ (x_ਪੋਰਟ_Get_Core_ID() >= 2) {
        abort();
    }

    // Wait for CPU0 to start FreeRTOS before progressing
    ਬਾਹਰੀ ਅਸਥਿਰ ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੋਰਟ_x_Scheduler_Running[CONFIG_FREERTOS_ਸੰਖਿਆ_ਦਾ_CORES];
    ਜਦੋਂ (ਪੋਰਟ_x_Scheduler_Running[0] == 0) {
        ;
    }

    ਲੜੀਵਾਰ_app_startup_event(ESP_SYS_EVENT_PRE_SCHEDULER);

    ESP_EARLY_LOGD(APP_ਸ਼ੁਰੂ_ਟੈਗ, "Starting scheduler on CPU%d", x_ਪੋਰਟ_Get_Core_ID());
    x_ਪੋਰਟ_ਸ਼ੁਰੂ_Scheduler();
    abort(); // Only get to here if FreeRTOS somehow very broken
}
#ਜੇ_ਅੰਤ // !CONFIG_FREERTOS_UNICORE

/* ---------------------------------------------------- Main Task ------------------------------------------------------
 * - main_task is a daemon task created by CPU0 before it starts FreeRTOS
 *      - Pinned to CPU(ESP_TASK_MAIN_CORE)
 *      - Priority of ESP_TASK_MAIN_PRIO
 * - Used to dispatch "void app_main(void)" provided by the application
 * - main_task will self delete if app_main returns
 * ------------------------------------------------------------------------------------------------------------------ */

ESP_LOG_ATTR_ਟੈਗ(MAIN_ਟੈਗ, "main_task");

/* This function has to guarantee that all CPUs have finished the FreeRTOS initialization
* and the startup stack is not in use.
*/
ਸਥਿਰ ਖਾਲੀ wait_for_all_cores_ready(ਖਾਲੀ)
{
#ਜੇ !CONFIG_FREERTOS_UNICORE
    ਬਾਹਰੀ ਅਸਥਿਰ ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੋਰਟ_ux_Core_Startup_ਪੂਰਾ[CONFIG_FREERTOS_ਸੰਖਿਆ_ਦਾ_CORES];
    bool all_cpus_has_been_started;
    ਕਰੋ {
        all_cpus_has_been_started = true;
        ਲਈ (ਪੂਰਨ cpu = 0; cpu < CONFIG_FREERTOS_ਸੰਖਿਆ_ਦਾ_CORES; cpu++) {
            all_cpus_has_been_started &= ਪੋਰਟ_ux_Core_Startup_ਪੂਰਾ[cpu] != 0;
        }
    } ਜਦੋਂ (!all_cpus_has_been_started);
#ਜੇ_ਅੰਤ // !CONFIG_FREERTOS_UNICORE
}

ਸਥਿਰ ਖਾਲੀ main_task(ਖਾਲੀ* ਦਲੀਲਾਂ)
{
    ESP_LOGI(MAIN_ਟੈਗ, "Started on CPU%d", (ਪੂਰਨ)x_ਪੋਰਟ_Get_Core_ID());

    wait_for_all_cores_ready();

    ਲੜੀਵਾਰ_app_startup_event(ESP_SYS_EVENT_PRE_APP_MAIN);

    /*
    Note: Be careful when changing the "Calling app_main()" log below as multiple pytest scripts expect this log as a
    start-of-application marker.
    */
    ESP_LOGI(MAIN_ਟੈਗ, "Calling app_main()");
    ਬਾਹਰੀ ਖਾਲੀ app_main(ਖਾਲੀ);
    app_main();
    ESP_LOGI(MAIN_ਟੈਗ, "Returned from app_main()");
    vTaskDelete(ਨਿਰਾ);
}
