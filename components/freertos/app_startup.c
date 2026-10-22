/*
 * SPDX-FileCopyrightText: 2022-2026 Espressif Systems (Shanghai) CO LTD
 *
 * SPDX-License-Identifier: Apache-2.0
 */

#अन्तर्भाव "sdkconfig.h"
#अन्तर्भाव <stddef.h>
#अन्तर्भाव <assert.h>
#अन्तर्भाव "esp_task.h"
#अन्तर्भाव "esp_log.h"
#अन्तर्भाव "freertos/FreeRTOS.h"
#अन्तर्भाव "freertos/task.h"
#अन्तर्भाव "freertos/portmacro.h"
#अन्तर्भाव "esp_private/esp_sys_event_internal.h"

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
#यदि CONFIG_FREERTOS_UNICORE != CONFIG_ESP_SYSTEM_SINGLE_CORE_MODE
#error "AMP not supported. FreeRTOS number of cores and system number of cores must be identical"
#endif

// -------------------- Declarations -----------------------

स्थिर शून्य main_task(शून्य* args);
ESP_LOG_ATTR_TAG(APP_START_TAG, "app_start");

स्थिर शून्य run_app_startup_event(esp_sys_event_id_t id)
{
    ESP_SYS_EVENT_FOREACH(handler, id) {
        ESP_ERROR_CHECK(handler->handler(रिक्त, रिक्त));
    }
}

// ------------------ CPU0 App Startup ---------------------

शून्य esp_startup_start_app(शून्य)
{
    run_app_startup_event(ESP_SYS_EVENT_PRE_SCHEDULER);

    BaseType_t res = xTaskCreatePinnedToCore(main_task, "main",
                                             ESP_TASK_MAIN_चळत, रिक्त,
                                             ESP_TASK_MAIN_PRIO, रिक्त, ESP_TASK_MAIN_CORE);
    प्रतिज्ञा(res == pdTRUE);
    (शून्य)res;

    /*
    If a particular FreeRTOS port has port/arch specific OS startup behavior, they can implement a function of type
    "void port_start_app_hook(void)" in their `port.c` files. This function will be called below, thus allowing each
    FreeRTOS port to implement port specific app startup behavior.

    Deprecated: Register a PRE_SCHEDULER handler with priority 999 to preserve this ordering.
    */
    // TODO: IDF-16135
    शून्य __attribute__((weak, deprecated("port_start_app_hook is deprecated and will be removed in IDF 7.0. Use ESP_PRE_SCHEDULER_HANDLER_REGISTER instead.")))
    port_start_app_hook(शून्य);
#pragma GCC diagnostic ढकला
#pragma GCC diagnostic ignored "-Wdeprecated-declarations"
    यदि (port_start_app_hook != रिक्त) {
        ESP_EARLY_LOGW(APP_START_TAG,
                       "port_start_app_hook is deprecated and will be removed in IDF 7.0. "
                       "Use ESP_PRE_SCHEDULER_HANDLER_REGISTER instead.");
        port_start_app_hook();
    }
#pragma GCC diagnostic काढा

    ESP_EARLY_LOGD(APP_START_TAG, "Starting scheduler on CPU0");
    vTaskStartScheduler();
}

// --------------- CPU[1:N-1] App Startup ------------------

#यदि !CONFIG_FREERTOS_UNICORE
शून्य esp_startup_start_app_other_cores(शून्य)
{
    // For now, we only support up to two core: 0 and 1.
    यदि (xPortGetCoreID() >= 2) {
        abort();
    }

    // Wait for CPU0 to start FreeRTOS before progressing
    बाह्य चञ्चल unsigned port_xSchedulerRunning[CONFIG_FREERTOS_संख्या_OF_CORES];
    यावत् (port_xSchedulerRunning[0] == 0) {
        ;
    }

    run_app_startup_event(ESP_SYS_EVENT_PRE_SCHEDULER);

    ESP_EARLY_LOGD(APP_START_TAG, "Starting scheduler on CPU%d", xPortGetCoreID());
    xPortStartScheduler();
    abort(); // Only get to here if FreeRTOS somehow very broken
}
#endif // !CONFIG_FREERTOS_UNICORE

/* ---------------------------------------------------- Main Task ------------------------------------------------------
 * - main_task is a daemon task created by CPU0 before it starts FreeRTOS
 *      - Pinned to CPU(ESP_TASK_MAIN_CORE)
 *      - Priority of ESP_TASK_MAIN_PRIO
 * - Used to dispatch "void app_main(void)" provided by the application
 * - main_task will self delete if app_main returns
 * ------------------------------------------------------------------------------------------------------------------ */

ESP_LOG_ATTR_TAG(MAIN_TAG, "main_task");

/* This function has to guarantee that all CPUs have finished the FreeRTOS initialization
* and the startup stack is not in use.
*/
स्थिर शून्य wait_for_all_cores_ready(शून्य)
{
#यदि !CONFIG_FREERTOS_UNICORE
    बाह्य चञ्चल unsigned port_uxCoreStartupDone[CONFIG_FREERTOS_संख्या_OF_CORES];
    bool all_cpus_has_been_started;
    कुरु {
        all_cpus_has_been_started = true;
        प्रति (पूर्णाङ्क cpu = 0; cpu < CONFIG_FREERTOS_संख्या_OF_CORES; cpu++) {
            all_cpus_has_been_started &= port_uxCoreStartupDone[cpu] != 0;
        }
    } यावत् (!all_cpus_has_been_started);
#endif // !CONFIG_FREERTOS_UNICORE
}

स्थिर शून्य main_task(शून्य* args)
{
    ESP_LOGI(MAIN_TAG, "Started on CPU%d", (पूर्णाङ्क)xPortGetCoreID());

    wait_for_all_cores_ready();

    run_app_startup_event(ESP_SYS_EVENT_PRE_APP_MAIN);

    /*
    Note: Be careful when changing the "Calling app_main()" log below as multiple pytest scripts expect this log as a
    start-of-application marker.
    */
    ESP_LOGI(MAIN_TAG, "Calling app_main()");
    बाह्य शून्य app_main(शून्य);
    app_main();
    ESP_LOGI(MAIN_TAG, "Returned from app_main()");
    vTaskDelete(रिक्त);
}
