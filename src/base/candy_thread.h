#ifndef CANDY_THREAD_H_
#define CANDY_THREAD_H_

typedef void (*candy_thread_fn) (void*);

#if defined CANDY_HAVE_WINDOWS
#include "candy_thread_win.h"
#else
#include "candy_thread_posix.h"
#endif

void candy_thread_start(struct candy_thread*,candy_thread_fn fn,void *arg);
thread_id candy_thread_self();
thread_id candy_thread_id(struct candy_thread*);
void candy_thread_stop(struct candy_thread*);

void candy_mutex_init(struct candy_mutex*);
void candy_mutex_lock(struct candy_mutex*);
void candy_mutex_unlock(struct candy_mutex*);
void candy_mutex_destroy(struct candy_mutex*);

#endif
