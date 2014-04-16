

#include "./candy_glock.h"

#if defined CANDY_HAVE_WINDOWS

#include "candy_window.h"

static LONG candy_glock_initialised = 0;
static CRITICAL_SECTION candy_glock_cs;

static void candy_glock_init (void)
{
    if (InterlockedCompareExchange (&candy_glock_initialised, 1, 0) == 0)
        InitializeCriticalSection (&candy_glock_cs);
}

void candy_glock_lock (void)
{
    candy_glock_init ();
    EnterCriticalSection (&candy_glock_cs);
}

void candy_glock_unlock (void)
{
    candy_glock_init ();
    LeaveCriticalSection (&candy_glock_cs);
}

#else

#include "candy/candy_log.h"
#include <pthread.h>

static pthread_mutex_t candy_glock_mutex = PTHREAD_MUTEX_INITIALIZER;

void candy_glock_lock (void)
{
    int rc;

    rc = pthread_mutex_lock (&candy_glock_mutex);
    CANDY_CHECK(rc == 0);
}

void candy_glock_unlock (void)
{
    int rc;

    rc = pthread_mutex_unlock (&candy_glock_mutex);
    CANDY_CHECK(rc == 0);
}

#endif


