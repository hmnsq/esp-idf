/*
 * SPDX-FileCopyrightText: 2025-2026 Espressif Systems (Shanghai) CO LTD
 *
 * SPDX-License-Identifier: Apache-2.0
 */
#அந்தர்பாவ <pthread.h>
#அந்தர்பாவ <stdlib.h>
#அந்தர்பாவ <errno.h>
#அந்தர்பாவ <assert.h>
#அந்தர்பாவ "wait_for_event.h"

/*-----------------------------------------------------------*/
/* Create a new event */
event_t *event_create(வெறுமை)
{
    event_t * ev = malloc(அளவு(event_t));
    ப்ரதிஜ்ஞா(ev != வெற்று);

    ev->event_triggered = false;
    pthread_mutex_init(&ev->mutex, வெற்று);
    pthread_cond_init(&ev->cond, வெற்று);

    திரும்பு ev;
}

/*-----------------------------------------------------------*/
/* Delete an event */
வெறுமை event_delete(event_t *ev)
{
    pthread_mutex_destroy(&ev->mutex);
    pthread_cond_destroy(&ev->cond);
    விடுவி(ev);
}

/*-----------------------------------------------------------*/
/* Wait for event indefinitely (cooperative blocking) */
bool event_wait(event_t *ev)
{
    pthread_mutex_lock(&ev->mutex);

    போது (!ev->event_triggered)
    {
        pthread_cond_wait(&ev->cond, &ev->mutex);
    }

    ev->event_triggered = false;
    pthread_mutex_unlock(&ev->mutex);
    திரும்பு true;
}

/*-----------------------------------------------------------*/
/* Wait for event with timeout (milliseconds) */
bool event_wait_timed(event_t *ev, time_t ms)
{
    கட்டமைப்பு timespec ts;
    முழு ret = 0;

    clock_gettime(CLOCK_REALTIME, &ts);
    ts.tv_sec  += ms / 1000;
    ts.tv_nsec += (ms % 1000) * 1000000;

    /* Normalize tv_nsec in case it exceeds 1,000,000,000 */
    எனில் (ts.tv_nsec >= 1000000000L) {
        ts.tv_sec += ts.tv_nsec / 1000000000L;
        ts.tv_nsec = ts.tv_nsec % 1000000000L;
    }

    pthread_mutex_lock(&ev->mutex);

    போது (!ev->event_triggered && ret == 0)
    {
        ret = pthread_cond_timedwait(&ev->cond, &ev->mutex, &ts);
        எனில் (ret == ETIMEDOUT)
        {
            ev->event_triggered = false;
            pthread_mutex_unlock(&ev->mutex);
            திரும்பு false;
        }
    }

    ev->event_triggered = false;
    pthread_mutex_unlock(&ev->mutex);
    திரும்பு true;
}

/*-----------------------------------------------------------*/
/* Signal / resume an event */
வெறுமை event_signal(event_t *ev)
{
    pthread_mutex_lock(&ev->mutex);
    ev->event_triggered = true;
    pthread_cond_signal(&ev->cond);
    pthread_mutex_unlock(&ev->mutex);
}

/*-----------------------------------------------------------*/
/* Discard a pending signal that was never consumed by a wait */
வெறுமை event_clear(event_t *ev)
{
    pthread_mutex_lock(&ev->mutex);
    ev->event_triggered = false;
    pthread_mutex_unlock(&ev->mutex);
}
