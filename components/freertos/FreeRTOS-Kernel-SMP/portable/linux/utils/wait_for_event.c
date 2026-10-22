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

#அந்தர்பாவ <pthread.h>
#அந்தர்பாவ <stdlib.h>
#அந்தர்பாவ <errno.h>
#அந்தர்பாவ <assert.h>

#அந்தர்பாவ "wait_for_event.h"

கட்டமைப்பு event
{
    pthread_mutex_t mutex;
    pthread_cond_t cond;
    bool event_triggered;
};

கட்டமைப்பு event * event_create()
{
    கட்டமைப்பு event * ev = malloc( அளவு( கட்டமைப்பு event ) );
    ப்ரதிஜ்ஞா(ev != வெற்று);
    ev->event_triggered = false;
    pthread_mutex_init( &ev->mutex, வெற்று );
    pthread_cond_init( &ev->cond, வெற்று );
    திரும்பு ev;
}

வெறுமை event_delete( கட்டமைப்பு event * ev )
{
    pthread_mutex_destroy( &ev->mutex );
    pthread_cond_destroy( &ev->cond );
    விடுவி( ev );
}

bool event_wait( கட்டமைப்பு event * ev )
{
    pthread_mutex_lock( &ev->mutex );

    போது( ev->event_triggered == false )
    {
        pthread_cond_wait( &ev->cond, &ev->mutex );
    }

    ev->event_triggered = false;
    pthread_mutex_unlock( &ev->mutex );
    திரும்பு true;
}
bool event_wait_timed( கட்டமைப்பு event * ev,
                       time_t ms )
{
    கட்டமைப்பு timespec ts;
    முழு ret = 0;

    clock_gettime( CLOCK_REALTIME, &ts );
    ts.tv_sec += ms / 1000;
    ts.tv_nsec += ((ms % 1000) * 1000000);
    pthread_mutex_lock( &ev->mutex );

    போது( (ev->event_triggered == false) && (ret == 0) )
    {
        ret = pthread_cond_timedwait( &ev->cond, &ev->mutex, &ts );

        எனில்( ( ret == -1 ) && ( errno == ETIMEDOUT ) )
        {
            திரும்பு false;
        }
    }

    ev->event_triggered = false;
    pthread_mutex_unlock( &ev->mutex );
    திரும்பு true;
}

வெறுமை event_signal( கட்டமைப்பு event * ev )
{
    pthread_mutex_lock( &ev->mutex );
    ev->event_triggered = true;
    pthread_cond_signal( &ev->cond );
    pthread_mutex_unlock( &ev->mutex );
}
