/*
 * SPDX-FileCopyrightText: 2026 Espressif Systems (Shanghai) CO LTD
 * SPDX-License-Identifier: Apache-2.0
 */

#निर्देशः once

#अन्तर्भाव <pthread.h>

#यदि_परिभाषितम् __cplusplus
बाह्य "C" {
#यदि_अन्तः

प्रकारनिर्देश संरचना thread *thread_hdl;

शून्य linux_पत्तनम्_सज्जता_backtrace_signal(शून्य);
शून्य linux_पत्तनम्_print_backtrace(शून्य);
pthread_t linux_पत्तनम्_get_scheduled_task_pthread(शून्य);

#यदि_परिभाषितम् __cplusplus
}
#यदि_अन्तः
