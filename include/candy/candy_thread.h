#ifndef CANDY_THREAD_H_
#define CANDY_THREAD_H_
#ifdef __cplusplus
extern "C" {
#endif

#include "./candy_export.h"

#define CANDY_NULL_THREAD_ID 0
typedef void (*candy_thread_fn) (void*);

struct candy_thread;
struct candy_mutex;
struct candy_cond;
struct candy_rwlock;
typedef struct candy_thread* candy_thread_t;
typedef struct candy_mutex* candy_mutex_t;
typedef struct candy_cond* candy_cond_t;
typedef struct candy_rwlock* candy_rwlock_t;

CANDY_EXPORT void candy_thread_start(candy_thread_t*,candy_thread_fn fn,void *arg);
CANDY_EXPORT int candy_thread_is_in(candy_thread_t self);
CANDY_EXPORT void candy_thread_wait(candy_thread_t);
CANDY_EXPORT void candy_thread_stop(candy_thread_t);

CANDY_EXPORT void candy_mutex_init(candy_mutex_t*);
CANDY_EXPORT void candy_mutex_lock(candy_mutex_t);
CANDY_EXPORT void candy_mutex_unlock(candy_mutex_t);
CANDY_EXPORT void candy_mutex_destroy(candy_mutex_t);

CANDY_EXPORT void candy_cond_init(candy_cond_t*);
CANDY_EXPORT void candy_cond_wait(candy_cond_t);
CANDY_EXPORT void candy_cond_notify(candy_cond_t);
CANDY_EXPORT void candy_cond_destroy(candy_cond_t);

CANDY_EXPORT void candy_rwlock_init(candy_rwlock_t*);
CANDY_EXPORT void candy_rwlock_rlock(candy_rwlock_t);
CANDY_EXPORT void candy_rwlock_runlock(candy_rwlock_t);
CANDY_EXPORT void candy_rwlock_wlock(candy_rwlock_t);
CANDY_EXPORT void candy_rwlock_wunlock(candy_rwlock_t);
CANDY_EXPORT void candy_rwlock_destroy(candy_rwlock_t);

#ifdef __cplusplus
}
#endif
#endif
