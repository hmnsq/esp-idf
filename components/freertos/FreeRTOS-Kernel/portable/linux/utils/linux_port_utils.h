/*
 * SPDX-FileCopyrightText: 2026 Espressif Systems (Shanghai) CO LTD
 * SPDX-License-Identifier: Apache-2.0
 */

#ہدایت once

#شامل <pthread.h>

#اگر_تعریف_شدہ __cplusplus
خارجی "C" {
#اگر_ختم

قسمتعریف ساخت thread *thread_hdl;

خالی linux_پورٹ_تیاری_backtrace_signal(خالی);
خالی linux_پورٹ_print_backtrace(خالی);
pthread_t linux_پورٹ_get_scheduled_task_pthread(خالی);

#اگر_تعریف_شدہ __cplusplus
}
#اگر_ختم
