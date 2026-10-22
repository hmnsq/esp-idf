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

#ಅನ್ತರ್ಭಾವ <pthread.h>
#ಅನ್ತರ್ಭಾವ <stdlib.h>
#ಅನ್ತರ್ಭಾವ <errno.h>
#ಅನ್ತರ್ಭಾವ <assert.h>

#ಅನ್ತರ್ಭಾವ "wait_for_event.h"

ರಚನೆ event
{
    pthread_mutex_t mutex;
    pthread_cond_t cond;
    bool event_triggered;
};

ರಚನೆ event * event_create()
{
    ರಚನೆ event * ev = ಸ್ಮರಣೆ_ಕೊಡು( ಗಾತ್ರ( ರಚನೆ event ) );
    ಪ್ರತಿಜ್ಞಾ(ev != ಖಾಲಿ);
    ev->event_triggered = false;
    pthread_mutex_init( &ev->mutex, ಖಾಲಿ );
    pthread_cond_init( &ev->cond, ಖಾಲಿ );
    ಹಿಂತಿರುಗಿ ev;
}

ಶೂನ್ಯ event_delete( ರಚನೆ event * ev )
{
    pthread_mutex_destroy( &ev->mutex );
    pthread_cond_destroy( &ev->cond );
    ಬಿಡುಗಡೆ( ev );
}

bool event_wait( ರಚನೆ event * ev )
{
    pthread_mutex_lock( &ev->mutex );

    ಆಗ( ev->event_triggered == false )
    {
        pthread_cond_wait( &ev->cond, &ev->mutex );
    }

    ev->event_triggered = false;
    pthread_mutex_unlock( &ev->mutex );
    ಹಿಂತಿರುಗಿ true;
}
bool event_wait_timed( ರಚನೆ event * ev,
                       time_t ms )
{
    ರಚನೆ timespec ts;
    ಪೂರ್ಣಾಂಕ ret = 0;

    clock_gettime( CLOCK_REALTIME, &ts );
    ts.tv_sec += ms / 1000;
    ts.tv_nsec += ((ms % 1000) * 1000000);
    pthread_mutex_lock( &ev->mutex );

    ಆಗ( (ev->event_triggered == false) && (ret == 0) )
    {
        ret = pthread_cond_timedwait( &ev->cond, &ev->mutex, &ts );

        ಆದರೆ( ( ret == -1 ) && ( errno == ETIMEDOUT ) )
        {
            ಹಿಂತಿರುಗಿ false;
        }
    }

    ev->event_triggered = false;
    pthread_mutex_unlock( &ev->mutex );
    ಹಿಂತಿರುಗಿ true;
}

ಶೂನ್ಯ event_signal( ರಚನೆ event * ev )
{
    pthread_mutex_lock( &ev->mutex );
    ev->event_triggered = true;
    pthread_cond_signal( &ev->cond );
    pthread_mutex_unlock( &ev->mutex );
}
