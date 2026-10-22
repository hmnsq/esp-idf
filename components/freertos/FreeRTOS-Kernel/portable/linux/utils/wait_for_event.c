/*
 * SPDX-FileCopyrightText: 2025-2026 Espressif Systems (Shanghai) CO LTD
 *
 * SPDX-License-Identifier: Apache-2.0
 */
#ಅನ್ತರ್ಭಾವ <pthread.h>
#ಅನ್ತರ್ಭಾವ <stdlib.h>
#ಅನ್ತರ್ಭಾವ <errno.h>
#ಅನ್ತರ್ಭಾವ <assert.h>
#ಅನ್ತರ್ಭಾವ "wait_for_event.h"

/*-----------------------------------------------------------*/
/* Create a new event */
event_t *event_create(ಶೂನ್ಯ)
{
    event_t * ev = malloc(ಪರಿಮಾಣ(event_t));
    ಪ್ರತಿಜ್ಞಾ(ev != ರಿಕ್ತ);

    ev->event_triggered = false;
    pthread_mutex_init(&ev->mutex, ರಿಕ್ತ);
    pthread_cond_init(&ev->cond, ರಿಕ್ತ);

    ಪ್ರತ್ಯಾಗಮ ev;
}

/*-----------------------------------------------------------*/
/* Delete an event */
ಶೂನ್ಯ event_delete(event_t *ev)
{
    pthread_mutex_destroy(&ev->mutex);
    pthread_cond_destroy(&ev->cond);
    ಮುಕ್ತ(ev);
}

/*-----------------------------------------------------------*/
/* Wait for event indefinitely (cooperative blocking) */
bool event_wait(event_t *ev)
{
    pthread_mutex_lock(&ev->mutex);

    ಯಾವತ್ (!ev->event_triggered)
    {
        pthread_cond_wait(&ev->cond, &ev->mutex);
    }

    ev->event_triggered = false;
    pthread_mutex_unlock(&ev->mutex);
    ಪ್ರತ್ಯಾಗಮ true;
}

/*-----------------------------------------------------------*/
/* Wait for event with timeout (milliseconds) */
bool event_wait_timed(event_t *ev, time_t ms)
{
    ಸಂರಚನಾ timespec ts;
    ಪೂರ್ಣಾಙ್ಕ ret = 0;

    clock_gettime(CLOCK_REALTIME, &ts);
    ts.tv_sec  += ms / 1000;
    ts.tv_nsec += (ms % 1000) * 1000000;

    /* Normalize tv_nsec in case it exceeds 1,000,000,000 */
    ಯದಿ (ts.tv_nsec >= 1000000000L) {
        ts.tv_sec += ts.tv_nsec / 1000000000L;
        ts.tv_nsec = ts.tv_nsec % 1000000000L;
    }

    pthread_mutex_lock(&ev->mutex);

    ಯಾವತ್ (!ev->event_triggered && ret == 0)
    {
        ret = pthread_cond_timedwait(&ev->cond, &ev->mutex, &ts);
        ಯದಿ (ret == ETIMEDOUT)
        {
            ev->event_triggered = false;
            pthread_mutex_unlock(&ev->mutex);
            ಪ್ರತ್ಯಾಗಮ false;
        }
    }

    ev->event_triggered = false;
    pthread_mutex_unlock(&ev->mutex);
    ಪ್ರತ್ಯಾಗಮ true;
}

/*-----------------------------------------------------------*/
/* Signal / resume an event */
ಶೂನ್ಯ event_signal(event_t *ev)
{
    pthread_mutex_lock(&ev->mutex);
    ev->event_triggered = true;
    pthread_cond_signal(&ev->cond);
    pthread_mutex_unlock(&ev->mutex);
}

/*-----------------------------------------------------------*/
/* Discard a pending signal that was never consumed by a wait */
ಶೂನ್ಯ event_clear(event_t *ev)
{
    pthread_mutex_lock(&ev->mutex);
    ev->event_triggered = false;
    pthread_mutex_unlock(&ev->mutex);
}
