#ifndef CANDY_THREAD_H_
#define CANDY_THREAD_H_
#include "./candy_export.h"

 
typedef void (*candy_thread_fn) (void*);

#if defined CANDY_HAVE_WINDOWS
#include "candy_thread_win.h"
#else
#include "candy_thread_posix.h"
#endif

CANDY_EXPORT void candy_thread_start(struct candy_thread*,candy_thread_fn fn,void *arg);
CANDY_EXPORT thread_id candy_thread_self();
CANDY_EXPORT thread_id candy_thread_id(struct candy_thread*);
CANDY_EXPORT void candy_thread_stop(struct candy_thread*);

CANDY_EXPORT void candy_mutex_init(struct candy_mutex*);
CANDY_EXPORT void candy_mutex_lock(struct candy_mutex*);
CANDY_EXPORT void candy_mutex_unlock(struct candy_mutex*);
CANDY_EXPORT void candy_mutex_destroy(struct candy_mutex*);

#endif
