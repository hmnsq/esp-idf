/*
 * SPDX-FileCopyrightText: 2026 Espressif Systems (Shanghai) CO LTD
 *
 * SPDX-License-Identifier: Apache-2.0
 */

/*
 * On Xtensa unicore, portYIELD_WITHIN_API() only pends a yield and returns.
 * vTaskDelete(NULL) must not come back to the deleted task: under Clang -O2
 * that continuation can fall off the task entry and panic with
 * IllegalInstruction. See https://github.com/espressif/esp-idf/issues/18460
 *
 * The child body is only vTaskDelete(), matching the original repro. Extra
 * code after vTaskDelete() (a flag store or busy-loop) is enough work that
 * the deferred yield interrupt often wins, so those checks miss the bug.
 *
 * app_main then returns so main_task also self-deletes (same kernel path).
 * A lower-priority heartbeat is created after PASS and must keep printing
 * if that second delete did not panic.
 */
#அந்தர்பாவ <array>
#அந்தர்பாவ <cstdio>

#அந்தர்பாவ "freertos/FreeRTOS.h"
#அந்தர்பாவ "freertos/task.h"

நாமாகாஶ {

StaticTask_t s_self_tcb;
StaticTask_t s_heartbeat_tcb;
std::array<அடுக்கு_Type_t, 4096> s_self_அடுக்கு;
std::array<அடுக்கு_Type_t, 4096> s_heartbeat_அடுக்கு;

வெறுமை self_delete_must_not_return(வெறுமை *arg)
{
    (வெறுமை)arg;
    vTaskDelete(nullptr);
}

வெறுமை heartbeat(வெறுமை *arg)
{
    (வெறுமை)arg;
    க்கு (;;) {
        std::printf("task_self_delete: still running\n");
        vTaskDelay(pdMS_TO_TICKS(100));
    }
}

} // namespace

வெளி "C" வெறுமை app_main(வெறுமை)
{
    TaskHandle_t self_del = xTaskCreateStatic(self_delete_must_not_return,
                                              "self_del",
                                              s_self_அடுக்கு.size(),
                                              nullptr,
                                              configMAX_PRIORITIES - 1,
                                              s_self_அடுக்கு.data(),
                                              &s_self_tcb);
    எனில் (self_del == nullptr) {
        std::printf("task_self_delete: FAIL create\n");
        திரும்பு;
    }

    /* Child was higher priority, so it has already run. Surviving to here
     * means it did not fall off its entry after vTaskDelete(). */
    std::printf("task_self_delete: PASS\n");

    TaskHandle_t hb = xTaskCreateStatic(heartbeat,
                                        "hb",
                                        s_heartbeat_அடுக்கு.size(),
                                        nullptr,
                                        tskIDLE_PRIORITY + 1,
                                        s_heartbeat_அடுக்கு.data(),
                                        &s_heartbeat_tcb);
    எனில் (hb == nullptr) {
        std::printf("task_self_delete: FAIL create\n");
        திரும்பு;
    }
}
