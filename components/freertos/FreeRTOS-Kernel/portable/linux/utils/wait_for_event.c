/*
 * SPDX-FileCopyrightText: 2025-2026 Espressif Systems (Shanghai) CO LTD
 *
 * SPDX-License-Identifier: Apache-2.0
 */
#अन्तर्भाव <pthread.h>
#अन्तर्भाव <stdlib.h>
#अन्तर्भाव <errno.h>
#अन्तर्भाव <assert.h>
#अन्तर्भाव "wait_for_event.h"

/*-----------------------------------------------------------*/
/* Create a new event */
event_t *event_create(शून्य)
{
    event_t * ev = दो_स्मृति(परिमाण(event_t));
    प्रतिज्ञा(ev != रिक्त);

    ev->event_triggered = false;
    pthread_mutex_init(&ev->mutex, रिक्त);
    pthread_cond_init(&ev->cond, रिक्त);

    फर्काऊ ev;
}

/*-----------------------------------------------------------*/
/* Delete an event */
शून्य event_delete(event_t *ev)
{
    pthread_mutex_destroy(&ev->mutex);
    pthread_cond_destroy(&ev->cond);
    मुक्त(ev);
}

/*-----------------------------------------------------------*/
/* Wait for event indefinitely (cooperative blocking) */
bool event_wait(event_t *ev)
{
    pthread_mutex_lock(&ev->mutex);

    जबसम्म (!ev->event_triggered)
    {
        pthread_cond_wait(&ev->cond, &ev->mutex);
    }

    ev->event_triggered = false;
    pthread_mutex_unlock(&ev->mutex);
    फर्काऊ true;
}

/*-----------------------------------------------------------*/
/* Wait for event with timeout (milliseconds) */
bool event_wait_timed(event_t *ev, समय_प्रकार ms)
{
    संरचना timespec ts;
    पूर्णाङ्क ret = 0;

    clock_gettime(CLOCK_REALTIME, &ts);
    ts.tv_sec  += ms / 1000;
    ts.tv_nsec += (ms % 1000) * 1000000;

    /* Normalize tv_nsec in case it exceeds 1,000,000,000 */
    यदि (ts.tv_nsec >= 1000000000L) {
        ts.tv_sec += ts.tv_nsec / 1000000000L;
        ts.tv_nsec = ts.tv_nsec % 1000000000L;
    }

    pthread_mutex_lock(&ev->mutex);

    जबसम्म (!ev->event_triggered && ret == 0)
    {
        ret = pthread_cond_timedwait(&ev->cond, &ev->mutex, &ts);
        यदि (ret == ETIMEDOUT)
        {
            ev->event_triggered = false;
            pthread_mutex_unlock(&ev->mutex);
            फर्काऊ false;
        }
    }

    ev->event_triggered = false;
    pthread_mutex_unlock(&ev->mutex);
    फर्काऊ true;
}

/*-----------------------------------------------------------*/
/* Signal / resume an event */
शून्य event_signal(event_t *ev)
{
    pthread_mutex_lock(&ev->mutex);
    ev->event_triggered = true;
    pthread_cond_signal(&ev->cond);
    pthread_mutex_unlock(&ev->mutex);
}

/*-----------------------------------------------------------*/
/* Discard a pending signal that was never consumed by a wait */
शून्य event_clear(event_t *ev)
{
    pthread_mutex_lock(&ev->mutex);
    ev->event_triggered = false;
    pthread_mutex_unlock(&ev->mutex);
}
