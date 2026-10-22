/*
 * SPDX-FileCopyrightText: 2026 Espressif Systems (Shanghai) CO LTD
 * SPDX-License-Identifier: Apache-2.0
 */

#pragma once

#ಅನ್ತರ್ಭಾವ <pthread.h>

#ifdef __cplusplus
ಬಾಹ್ಯ "C" {
#endif

ಪ್ರಕಾರನಿರ್ದೇಶ ಸಂರಚನಾ thread *thread_hdl;

ಶೂನ್ಯ linux_port_setup_backtrace_signal(ಶೂನ್ಯ);
ಶೂನ್ಯ linux_port_print_backtrace(ಶೂನ್ಯ);
pthread_t linux_port_get_scheduled_task_pthread(ಶೂನ್ಯ);

#ifdef __cplusplus
}
#endif
