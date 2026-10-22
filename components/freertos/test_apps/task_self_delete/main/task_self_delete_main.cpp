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
#ضمن <array>
#ضمن <cstdio>

#ضمن "freertos/FreeRTOS.h"
#ضمن "freertos/task.h"

نطاق {

StaticTask_t s_self_tcb;
StaticTask_t s_heartbeat_tcb;
std::array<StackType_t, 4096> s_self_stack;
std::array<StackType_t, 4096> s_heartbeat_stack;

فراغ self_delete_must_not_return(فراغ *arg)
{
    (فراغ)arg;
    vTaskDelete(nullptr);
}

فراغ heartbeat(فراغ *arg)
{
    (فراغ)arg;
    لكل (;;) {
        std::printf("task_self_delete: still running\n");
        vTaskDelay(pdMS_TO_TICKS(100));
    }
}

} // namespace

خارجي "C" فراغ app_main(فراغ)
{
    TaskHandle_t self_del = xTaskCreateStatic(self_delete_must_not_return,
                                              "self_del",
                                              s_self_stack.size(),
                                              nullptr,
                                              configMAX_PRIORITIES - 1,
                                              s_self_stack.data(),
                                              &s_self_tcb);
    ܐܢ (self_del == nullptr) {
        std::printf("task_self_delete: FAIL create\n");
        ܦܢܐ;
    }

    /* Child was higher priority, so it has already run. Surviving to here
     * means it did not fall off its entry after vTaskDelete(). */
    std::printf("task_self_delete: PASS\n");

    TaskHandle_t hb = xTaskCreateStatic(heartbeat,
                                        "hb",
                                        s_heartbeat_stack.size(),
                                        nullptr,
                                        tskIDLE_PRIORITY + 1,
                                        s_heartbeat_stack.data(),
                                        &s_heartbeat_tcb);
    ܐܢ (hb == nullptr) {
        std::printf("task_self_delete: FAIL create\n");
        ܦܢܐ;
    }
}
