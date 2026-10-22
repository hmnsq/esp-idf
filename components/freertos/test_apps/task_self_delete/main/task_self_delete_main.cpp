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
#અન્તર્ભાવ <array>
#અન્તર્ભાવ <cstdio>

#અન્તર્ભાવ "freertos/FreeRTOS.h"
#અન્તર્ભાવ "freertos/task.h"

નામાકાશ {

StaticTask_t s_self_tcb;
StaticTask_t s_heartbeat_tcb;
std::array<ઢગલો_Type_t, 4096> s_self_ઢગલો;
std::array<ઢગલો_Type_t, 4096> s_heartbeat_ઢગલો;

શૂન્ય self_delete_must_not_return(શૂન્ય *arg)
{
    (શૂન્ય)arg;
    vTaskDelete(nullptr);
}

શૂન્ય heartbeat(શૂન્ય *arg)
{
    (શૂન્ય)arg;
    માટે (;;) {
        std::printf("task_self_delete: still running\n");
        vTaskDelay(pdMS_TO_TICKS(100));
    }
}

} // namespace

બાહ્ય "C" શૂન્ય app_main(શૂન્ય)
{
    TaskHandle_t self_del = xTaskCreateStatic(self_delete_must_not_return,
                                              "self_del",
                                              s_self_ઢગલો.size(),
                                              nullptr,
                                              configMAX_PRIORITIES - 1,
                                              s_self_ઢગલો.data(),
                                              &s_self_tcb);
    જો (self_del == nullptr) {
        std::printf("task_self_delete: FAIL create\n");
        પાછા;
    }

    /* Child was higher priority, so it has already run. Surviving to here
     * means it did not fall off its entry after vTaskDelete(). */
    std::printf("task_self_delete: PASS\n");

    TaskHandle_t hb = xTaskCreateStatic(heartbeat,
                                        "hb",
                                        s_heartbeat_ઢગલો.size(),
                                        nullptr,
                                        tskIDLE_PRIORITY + 1,
                                        s_heartbeat_ઢગલો.data(),
                                        &s_heartbeat_tcb);
    જો (hb == nullptr) {
        std::printf("task_self_delete: FAIL create\n");
        પાછા;
    }
}
