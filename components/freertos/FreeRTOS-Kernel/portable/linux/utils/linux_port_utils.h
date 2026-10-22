/*
 * SPDX-FileCopyrightText: 2026 Espressif Systems (Shanghai) CO LTD
 * SPDX-License-Identifier: Apache-2.0
 */

#pragma once

#अन्तर्भाव <pthread.h>

#ifdef __cplusplus
बाह्य "C" {
#endif

प्रकारनिर्देश संरचना thread *thread_hdl;

शून्य linux_port_setup_backtrace_signal(शून्य);
शून्य linux_port_print_backtrace(शून्य);
pthread_t linux_port_get_scheduled_task_pthread(शून्य);

#ifdef __cplusplus
}
#endif
