/*
 * SPDX-FileCopyrightText: 2022-2026 Espressif Systems (Shanghai) CO LTD
 *
 * SPDX-License-Identifier: Apache-2.0
 */

#ಅನ್ತರ್ಭಾವ "sdkconfig.h"
#ಅನ್ತರ್ಭಾವ <stddef.h>
#ಅನ್ತರ್ಭಾವ <assert.h>
#ಅನ್ತರ್ಭಾವ "esp_task.h"
#ಅನ್ತರ್ಭಾವ "esp_log.h"
#ಅನ್ತರ್ಭಾವ "freertos/FreeRTOS.h"
#ಅನ್ತರ್ಭಾವ "freertos/task.h"
#ಅನ್ತರ್ಭಾವ "freertos/portmacro.h"
#ಅನ್ತರ್ಭಾವ "esp_private/esp_sys_event_internal.h"

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
#ಯದಿ CONFIG_FREERTOS_UNICORE != CONFIG_ESP_SYSTEM_SINGLE_CORE_MODE
#error "AMP not supported. FreeRTOS number of cores and system number of cores must be identical"
#endif

// -------------------- Declarations -----------------------

ಸ್ಥಿರ ಶೂನ್ಯ main_task(ಶೂನ್ಯ* args);
ESP_LOG_ATTR_TAG(APP_START_TAG, "app_start");

ಸ್ಥಿರ ಶೂನ್ಯ run_app_startup_event(esp_sys_event_id_t id)
{
    ESP_SYS_EVENT_FOREACH(handler, id) {
        ESP_ERROR_CHECK(handler->handler(ರಿಕ್ತ, ರಿಕ್ತ));
    }
}

// ------------------ CPU0 App Startup ---------------------

ಶೂನ್ಯ esp_startup_start_app(ಶೂನ್ಯ)
{
    run_app_startup_event(ESP_SYS_EVENT_PRE_SCHEDULER);

    BaseType_t res = xTaskCreatePinnedToCore(main_task, "main",
                                             ESP_TASK_MAIN_ರಾಶಿ, ರಿಕ್ತ,
                                             ESP_TASK_MAIN_PRIO, ರಿಕ್ತ, ESP_TASK_MAIN_CORE);
    ಪ್ರತಿಜ್ಞಾ(res == pdTRUE);
    (ಶೂನ್ಯ)res;

    /*
    If a particular FreeRTOS port has port/arch specific OS startup behavior, they can implement a function of type
    "void port_start_app_hook(void)" in their `port.c` files. This function will be called below, thus allowing each
    FreeRTOS port to implement port specific app startup behavior.

    Deprecated: Register a PRE_SCHEDULER handler with priority 999 to preserve this ordering.
    */
    // TODO: IDF-16135
    ಶೂನ್ಯ __attribute__((weak, deprecated("port_start_app_hook is deprecated and will be removed in IDF 7.0. Use ESP_PRE_SCHEDULER_HANDLER_REGISTER instead.")))
    port_start_app_hook(ಶೂನ್ಯ);
#pragma GCC diagnostic ತಳ್ಳು
#pragma GCC diagnostic ignored "-Wdeprecated-declarations"
    ಯದಿ (port_start_app_hook != ರಿಕ್ತ) {
        ESP_EARLY_LOGW(APP_START_TAG,
                       "port_start_app_hook is deprecated and will be removed in IDF 7.0. "
                       "Use ESP_PRE_SCHEDULER_HANDLER_REGISTER instead.");
        port_start_app_hook();
    }
#pragma GCC diagnostic ತೆಗೆ

    ESP_EARLY_LOGD(APP_START_TAG, "Starting scheduler on CPU0");
    vTaskStartScheduler();
}

// --------------- CPU[1:N-1] App Startup ------------------

#ಯದಿ !CONFIG_FREERTOS_UNICORE
ಶೂನ್ಯ esp_startup_start_app_other_cores(ಶೂನ್ಯ)
{
    // For now, we only support up to two core: 0 and 1.
    ಯದಿ (xPortGetCoreID() >= 2) {
        abort();
    }

    // Wait for CPU0 to start FreeRTOS before progressing
    ಬಾಹ್ಯ ಚಞ್ಚಲ unsigned port_xSchedulerRunning[CONFIG_FREERTOS_ಸಂಖ್ಯೆ_OF_CORES];
    ಯಾವತ್ (port_xSchedulerRunning[0] == 0) {
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
ಸ್ಥಿರ ಶೂನ್ಯ wait_for_all_cores_ready(ಶೂನ್ಯ)
{
#ಯದಿ !CONFIG_FREERTOS_UNICORE
    ಬಾಹ್ಯ ಚಞ್ಚಲ unsigned port_uxCoreStartupDone[CONFIG_FREERTOS_ಸಂಖ್ಯೆ_OF_CORES];
    bool all_cpus_has_been_started;
    ಕುರು {
        all_cpus_has_been_started = true;
        ಪ್ರತಿ (ಪೂರ್ಣಾಙ್ಕ cpu = 0; cpu < CONFIG_FREERTOS_ಸಂಖ್ಯೆ_OF_CORES; cpu++) {
            all_cpus_has_been_started &= port_uxCoreStartupDone[cpu] != 0;
        }
    } ಯಾವತ್ (!all_cpus_has_been_started);
#endif // !CONFIG_FREERTOS_UNICORE
}

ಸ್ಥಿರ ಶೂನ್ಯ main_task(ಶೂನ್ಯ* args)
{
    ESP_LOGI(MAIN_TAG, "Started on CPU%d", (ಪೂರ್ಣಾಙ್ಕ)xPortGetCoreID());

    wait_for_all_cores_ready();

    run_app_startup_event(ESP_SYS_EVENT_PRE_APP_MAIN);

    /*
    Note: Be careful when changing the "Calling app_main()" log below as multiple pytest scripts expect this log as a
    start-of-application marker.
    */
    ESP_LOGI(MAIN_TAG, "Calling app_main()");
    ಬಾಹ್ಯ ಶೂನ್ಯ app_main(ಶೂನ್ಯ);
    app_main();
    ESP_LOGI(MAIN_TAG, "Returned from app_main()");
    vTaskDelete(ರಿಕ್ತ);
}
