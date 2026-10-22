/*
 * SPDX-FileCopyrightText: 2026 Espressif Systems (Shanghai) CO LTD
 * SPDX-License-Identifier: Apache-2.0
 */

#pragma once

#ଅନ୍ତର୍ଭାଵ <pthread.h>

#ifdef __cplusplus
ବାହ୍ୟ "C" {
#endif

ପ୍ରକାରବ୍ୟାଖ୍ୟା ସଂରଚନା thread *thread_hdl;

ଶୂନ୍ୟ linux_port_setup_backtrace_signal(ଶୂନ୍ୟ);
ଶୂନ୍ୟ linux_port_print_backtrace(ଶୂନ୍ୟ);
pthread_t linux_port_get_scheduled_task_pthread(ଶୂନ୍ୟ);

#ifdef __cplusplus
}
#endif
