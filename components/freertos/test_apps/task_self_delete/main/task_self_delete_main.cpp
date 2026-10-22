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
#समावेश <array>
#समावेश <cstdio>

#समावेश "freertos/FreeRTOS.h"
#समावेश "freertos/task.h"

namespace {

StaticTask_t s_self_tcb;
StaticTask_t s_heartbeat_tcb;
std::array<ढेर_Type_t, 4096> s_self_ढेर;
std::array<ढेर_Type_t, 4096> s_heartbeat_ढेर;

व्योम self_delete_must_not_return(व्योम *arg)
{
    (व्योम)arg;
    vTaskDelete(nullptr);
}

व्योम heartbeat(व्योम *arg)
{
    (व्योम)arg;
    क्रम (;;) {
        std::म_लिखो("task_self_delete: still running\n");
        vTaskDelay(pdMS_TO_TICKS(100));
    }
}

} // namespace

बाह्य "C" व्योम app_main(व्योम)
{
    TaskHandle_t self_del = xTaskCreateStatic(self_delete_must_not_return,
                                              "self_del",
                                              s_self_ढेर.size(),
                                              nullptr,
                                              configMAX_PRIORITIES - 1,
                                              s_self_ढेर.data(),
                                              &s_self_tcb);
    अगर (self_del == nullptr) {
        std::म_लिखो("task_self_delete: FAIL create\n");
        वापस;
    }

    /* Child was higher priority, so it has already run. Surviving to here
     * means it did not fall off its entry after vTaskDelete(). */
    std::म_लिखो("task_self_delete: PASS\n");

    TaskHandle_t hb = xTaskCreateStatic(heartbeat,
                                        "hb",
                                        s_heartbeat_ढेर.size(),
                                        nullptr,
                                        tskIDLE_PRIORITY + 1,
                                        s_heartbeat_ढेर.data(),
                                        &s_heartbeat_tcb);
    अगर (hb == nullptr) {
        std::म_लिखो("task_self_delete: FAIL create\n");
        वापस;
    }
}
