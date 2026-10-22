/*
 * SPDX-FileCopyrightText: 2022-2026 Espressif Systems (Shanghai) CO LTD
 *
 * SPDX-License-Identifier: Apache-2.0
 */

#অন্তর্ভাব "sdkconfig.h"
#অন্তর্ভাব <stddef.h>
#অন্তর্ভাব <assert.h>
#অন্তর্ভাব "esp_task.h"
#অন্তর্ভাব "esp_log.h"
#অন্তর্ভাব "freertos/FreeRTOS.h"
#অন্তর্ভাব "freertos/task.h"
#অন্তর্ভাব "freertos/portmacro.h"
#অন্তর্ভাব "esp_private/esp_sys_event_internal.h"

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
#যদি CONFIG_FREERTOS_UNICORE != CONFIG_ESP_SYSTEM_SINGLE_CORE_MODE
#error "AMP not supported. FreeRTOS number of cores and system number of cores must be identical"
#endif

// -------------------- Declarations -----------------------

স্থির শূন্য main_task(শূন্য* args);
ESP_LOG_ATTR_TAG(APP_START_TAG, "app_start");

স্থির শূন্য run_app_startup_event(esp_sys_event_id_t id)
{
    ESP_SYS_EVENT_FOREACH(handler, id) {
        ESP_ERROR_CHECK(handler->handler(ফাঁকা, ফাঁকা));
    }
}

// ------------------ CPU0 App Startup ---------------------

শূন্য esp_startup_start_app(শূন্য)
{
    run_app_startup_event(ESP_SYS_EVENT_PRE_SCHEDULER);

    BaseType_t res = xTaskCreatePinnedToCore(main_task, "main",
                                             ESP_TASK_MAIN_স্তূপ, ফাঁকা,
                                             ESP_TASK_MAIN_PRIO, ফাঁকা, ESP_TASK_MAIN_CORE);
    প্রতিজ্ঞা(res == pdTRUE);
    (শূন্য)res;

    /*
    If a particular FreeRTOS port has port/arch specific OS startup behavior, they can implement a function of type
    "void port_start_app_hook(void)" in their `port.c` files. This function will be called below, thus allowing each
    FreeRTOS port to implement port specific app startup behavior.

    Deprecated: Register a PRE_SCHEDULER handler with priority 999 to preserve this ordering.
    */
    // TODO: IDF-16135
    শূন্য __attribute__((weak, deprecated("port_start_app_hook is deprecated and will be removed in IDF 7.0. Use ESP_PRE_SCHEDULER_HANDLER_REGISTER instead.")))
    port_start_app_hook(শূন্য);
#pragma GCC diagnostic ঠেলো
#pragma GCC diagnostic ignored "-Wdeprecated-declarations"
    যদি (port_start_app_hook != ফাঁকা) {
        ESP_EARLY_LOGW(APP_START_TAG,
                       "port_start_app_hook is deprecated and will be removed in IDF 7.0. "
                       "Use ESP_PRE_SCHEDULER_HANDLER_REGISTER instead.");
        port_start_app_hook();
    }
#pragma GCC diagnostic বের_করো

    ESP_EARLY_LOGD(APP_START_TAG, "Starting scheduler on CPU0");
    vTaskStartScheduler();
}

// --------------- CPU[1:N-1] App Startup ------------------

#যদি !CONFIG_FREERTOS_UNICORE
শূন্য esp_startup_start_app_other_cores(শূন্য)
{
    // For now, we only support up to two core: 0 and 1.
    যদি (xPortGetCoreID() >= 2) {
        abort();
    }

    // Wait for CPU0 to start FreeRTOS before progressing
    বাহ্যিক অস্থির unsigned port_xSchedulerRunning[CONFIG_FREERTOS_সংখ্যা_OF_CORES];
    যখন (port_xSchedulerRunning[0] == 0) {
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
স্থির শূন্য wait_for_all_cores_ready(শূন্য)
{
#যদি !CONFIG_FREERTOS_UNICORE
    বাহ্যিক অস্থির unsigned port_uxCoreStartupDone[CONFIG_FREERTOS_সংখ্যা_OF_CORES];
    bool all_cpus_has_been_started;
    করো {
        all_cpus_has_been_started = true;
        জন্য (পূর্ণসংখ্যা cpu = 0; cpu < CONFIG_FREERTOS_সংখ্যা_OF_CORES; cpu++) {
            all_cpus_has_been_started &= port_uxCoreStartupDone[cpu] != 0;
        }
    } যখন (!all_cpus_has_been_started);
#endif // !CONFIG_FREERTOS_UNICORE
}

স্থির শূন্য main_task(শূন্য* args)
{
    ESP_LOGI(MAIN_TAG, "Started on CPU%d", (পূর্ণসংখ্যা)xPortGetCoreID());

    wait_for_all_cores_ready();

    run_app_startup_event(ESP_SYS_EVENT_PRE_APP_MAIN);

    /*
    Note: Be careful when changing the "Calling app_main()" log below as multiple pytest scripts expect this log as a
    start-of-application marker.
    */
    ESP_LOGI(MAIN_TAG, "Calling app_main()");
    বাহ্যিক শূন্য app_main(শূন্য);
    app_main();
    ESP_LOGI(MAIN_TAG, "Returned from app_main()");
    vTaskDelete(ফাঁকা);
}
