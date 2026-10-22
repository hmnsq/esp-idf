/*
 * SPDX-FileCopyrightText: 2026 Espressif Systems (Shanghai) CO LTD
 * SPDX-License-Identifier: Apache-2.0
 */

#pragma once

#অন্তর্ভাৱ <pthread.h>

#ifdef __cplusplus
বাহ্য "C" {
#endif

প্রকারনির্দেশ সংরচনা thread *thread_hdl;

শূন্য linux_port_setup_backtrace_signal(শূন্য);
শূন্য linux_port_print_backtrace(শূন্য);
pthread_t linux_port_get_scheduled_task_pthread(শূন্য);

#ifdef __cplusplus
}
#endif
