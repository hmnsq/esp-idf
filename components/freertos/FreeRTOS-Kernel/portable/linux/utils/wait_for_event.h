/*
 * SPDX-FileCopyrightText: 2021-2025 Amazon.com, Inc. or its affiliates
 *
 * SPDX-License-Identifier: MIT
 */
/*
 * FreeRTOS Kernel V10.4.6
 * Copyright (C) 2021 Amazon.com, Inc. or its affiliates.  All Rights Reserved.
 *
 * SPDX-License-Identifier: MIT
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy of
 * this software and associated documentation files (the "Software"), to deal in
 * the Software without restriction, including without limitation the rights to
 * use, copy, modify, merge, publish, distribute, sublicense, and/or sell copies of
 * the Software, and to permit persons to whom the Software is furnished to do so,
 * subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in all
 * copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY, FITNESS
 * FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR
 * COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER
 * IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN
 * CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
 *
 * https://www.FreeRTOS.org
 * https://github.com/FreeRTOS
 *
 */

#pragma once

#ifdef __cplusplus
बाह्य "C" {
#endif

#अन्तर्भाव <stdbool.h>
#अन्तर्भाव <time.h>

/**
 * @brief
 *
 */
प्रकारव्याख्या संरचना event
{
    pthread_mutex_t mutex;
    pthread_cond_t cond;
    bool event_triggered;
} event_t;


/**
 * @brief
 *
 * @return event_t*
 */
event_t *event_create(शून्य);

/**
 * @brief
 *
 * @param ev
 */
शून्य event_delete(event_t *ev);

/**
 * @brief
 *
 * @param ev
 * @return true
 * @return false
 */
bool event_wait(event_t *ev);

/**
 * @brief
 *
 * @param ev
 * @param ms
 * @return true
 * @return false
 */
bool event_wait_timed(event_t *ev, समय_प्रकार ms);

/**
 * @brief
 *
 * @param ev
 */
शून्य event_signal(event_t *ev);

/**
 * @brief Discard a pending signal that was never consumed by a wait
 *
 * @param ev
 */
शून्य event_clear(event_t *ev);

#ifdef __cplusplus
}
#endif
