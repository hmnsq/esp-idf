/*
 * SPDX-FileCopyrightText: 2021 Amazon.com, Inc. or its affiliates
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

#അന്തര്ഭാവ <pthread.h>
#അന്തര്ഭാവ <stdlib.h>
#അന്തര്ഭാവ <errno.h>
#അന്തര്ഭാവ <assert.h>

#അന്തര്ഭാവ "wait_for_event.h"

ഘടന event
{
    pthread_mutex_t mutex;
    pthread_cond_t cond;
    bool event_triggered;
};

ഘടന event * event_create()
{
    ഘടന event * ev = malloc( വലുപ്പം( ഘടന event ) );
    പ്രതിജ്ഞാ(ev != ശൂന്യ);
    ev->event_triggered = false;
    pthread_mutex_init( &ev->mutex, ശൂന്യ );
    pthread_cond_init( &ev->cond, ശൂന്യ );
    തിരികെ ev;
}

ശൂന്യം event_delete( ഘടന event * ev )
{
    pthread_mutex_destroy( &ev->mutex );
    pthread_cond_destroy( &ev->cond );
    സ്വതന്ത്രം( ev );
}

bool event_wait( ഘടന event * ev )
{
    pthread_mutex_lock( &ev->mutex );

    ആകുമ്പോൾ( ev->event_triggered == false )
    {
        pthread_cond_wait( &ev->cond, &ev->mutex );
    }

    ev->event_triggered = false;
    pthread_mutex_unlock( &ev->mutex );
    തിരികെ true;
}
bool event_wait_timed( ഘടന event * ev,
                       time_t ms )
{
    ഘടന timespec ts;
    പൂർണ്ണസംഖ്യ ret = 0;

    clock_gettime( CLOCK_REALTIME, &ts );
    ts.tv_sec += ms / 1000;
    ts.tv_nsec += ((ms % 1000) * 1000000);
    pthread_mutex_lock( &ev->mutex );

    ആകുമ്പോൾ( (ev->event_triggered == false) && (ret == 0) )
    {
        ret = pthread_cond_timedwait( &ev->cond, &ev->mutex, &ts );

        എങ്കിൽ( ( ret == -1 ) && ( errno == ETIMEDOUT ) )
        {
            തിരികെ false;
        }
    }

    ev->event_triggered = false;
    pthread_mutex_unlock( &ev->mutex );
    തിരികെ true;
}

ശൂന്യം event_signal( ഘടന event * ev )
{
    pthread_mutex_lock( &ev->mutex );
    ev->event_triggered = true;
    pthread_cond_signal( &ev->cond );
    pthread_mutex_unlock( &ev->mutex );
}
