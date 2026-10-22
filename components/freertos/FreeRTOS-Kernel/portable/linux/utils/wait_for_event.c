/*
 * SPDX-FileCopyrightText: 2025-2026 Espressif Systems (Shanghai) CO LTD
 *
 * SPDX-License-Identifier: Apache-2.0
 */
#അന്തര്ഭാവ <pthread.h>
#അന്തര്ഭാവ <stdlib.h>
#അന്തര്ഭാവ <errno.h>
#അന്തര്ഭാവ <assert.h>
#അന്തര്ഭാവ "wait_for_event.h"

/*-----------------------------------------------------------*/
/* Create a new event */
event_t *event_create(ശൂന്യം)
{
    event_t * ev = malloc(വലുപ്പം(event_t));
    പ്രതിജ്ഞാ(ev != ശൂന്യ);

    ev->event_triggered = false;
    pthread_mutex_init(&ev->mutex, ശൂന്യ);
    pthread_cond_init(&ev->cond, ശൂന്യ);

    തിരികെ ev;
}

/*-----------------------------------------------------------*/
/* Delete an event */
ശൂന്യം event_delete(event_t *ev)
{
    pthread_mutex_destroy(&ev->mutex);
    pthread_cond_destroy(&ev->cond);
    സ്വതന്ത്രം(ev);
}

/*-----------------------------------------------------------*/
/* Wait for event indefinitely (cooperative blocking) */
bool event_wait(event_t *ev)
{
    pthread_mutex_lock(&ev->mutex);

    ആകുമ്പോൾ (!ev->event_triggered)
    {
        pthread_cond_wait(&ev->cond, &ev->mutex);
    }

    ev->event_triggered = false;
    pthread_mutex_unlock(&ev->mutex);
    തിരികെ true;
}

/*-----------------------------------------------------------*/
/* Wait for event with timeout (milliseconds) */
bool event_wait_timed(event_t *ev, time_t ms)
{
    ഘടന timespec ts;
    പൂർണ്ണസംഖ്യ ret = 0;

    clock_gettime(CLOCK_REALTIME, &ts);
    ts.tv_sec  += ms / 1000;
    ts.tv_nsec += (ms % 1000) * 1000000;

    /* Normalize tv_nsec in case it exceeds 1,000,000,000 */
    എങ്കിൽ (ts.tv_nsec >= 1000000000L) {
        ts.tv_sec += ts.tv_nsec / 1000000000L;
        ts.tv_nsec = ts.tv_nsec % 1000000000L;
    }

    pthread_mutex_lock(&ev->mutex);

    ആകുമ്പോൾ (!ev->event_triggered && ret == 0)
    {
        ret = pthread_cond_timedwait(&ev->cond, &ev->mutex, &ts);
        എങ്കിൽ (ret == ETIMEDOUT)
        {
            ev->event_triggered = false;
            pthread_mutex_unlock(&ev->mutex);
            തിരികെ false;
        }
    }

    ev->event_triggered = false;
    pthread_mutex_unlock(&ev->mutex);
    തിരികെ true;
}

/*-----------------------------------------------------------*/
/* Signal / resume an event */
ശൂന്യം event_signal(event_t *ev)
{
    pthread_mutex_lock(&ev->mutex);
    ev->event_triggered = true;
    pthread_cond_signal(&ev->cond);
    pthread_mutex_unlock(&ev->mutex);
}

/*-----------------------------------------------------------*/
/* Discard a pending signal that was never consumed by a wait */
ശൂന്യം event_clear(event_t *ev)
{
    pthread_mutex_lock(&ev->mutex);
    ev->event_triggered = false;
    pthread_mutex_unlock(&ev->mutex);
}
