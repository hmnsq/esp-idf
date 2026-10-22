/*
 * SPDX-FileCopyrightText: 2026 Espressif Systems (Shanghai) CO LTD
 * SPDX-License-Identifier: Apache-2.0
 */

#pragma once

#ضمن <pthread.h>

#ifdef __cplusplus
خارجي "C" {
#endif

تعريفنوع بنية thread *thread_hdl;

فراغ linux_port_setup_backtrace_signal(فراغ);
فراغ linux_port_اطبع_backtrace(فراغ);
pthread_t linux_port_get_scheduled_task_pthread(فراغ);

#ifdef __cplusplus
}
#endif
