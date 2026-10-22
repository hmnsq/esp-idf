/*
 * SPDX-FileCopyrightText: 2026 Espressif Systems (Shanghai) CO LTD
 * SPDX-License-Identifier: Apache-2.0
 */

#pragma once

#അന്തര്ഭാവ <pthread.h>

#ifdef __cplusplus
ബാഹ്യ "C" {
#endif

തരനിർവചനം ഘടന thread *thread_hdl;

ശൂന്യം linux_port_setup_backtrace_signal(ശൂന്യം);
ശൂന്യം linux_port_print_backtrace(ശൂന്യം);
pthread_t linux_port_get_scheduled_task_pthread(ശൂന്യം);

#ifdef __cplusplus
}
#endif
