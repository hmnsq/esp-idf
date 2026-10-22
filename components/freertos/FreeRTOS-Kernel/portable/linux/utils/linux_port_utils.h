/*
 * SPDX-FileCopyrightText: 2026 Espressif Systems (Shanghai) CO LTD
 * SPDX-License-Identifier: Apache-2.0
 */

#pragma once

#شامل <pthread.h>

#ifdef __cplusplus
بیرونی "C" {
#endif

تعریفنوع ساختار thread *thread_hdl;

تهی linux_port_setup_backtrace_signal(تهی);
تهی linux_port_print_backtrace(تهی);
pthread_t linux_port_get_scheduled_task_pthread(تهی);

#ifdef __cplusplus
}
#endif
