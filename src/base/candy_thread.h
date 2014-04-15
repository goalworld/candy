#ifndef CANDY_THREAD_H_
#define CANDY_THREAD_H_
#include "./candy_export.h"

#define CANDY_NULL_THREAD_ID 0
typedef void (*candy_thread_fn) (void*);

#if defined CANDY_HAVE_WINDOWS
	#include "candy_window.h"
	typedef DWORD thread_id;
	
	struct candy_thread{
		candy_thread_fn fn;
		void * arg;
		HANDLE handle;
		thread_id id;
	};

	struct candy_mutex{
		CRITICAL_SECTION cs;
	};

	struct candy_cond{
		HANDLE hevent;
	};
#else
	#include <pthread.h>
	typedef pthread_t thread_id;

	struct candy_thread{
		candy_thread_fn fn;
		void * arg;
		pthread_t pid;
	};

	struct candy_mutex{
		pthread_mutex_t mtx;
	};

	struct candy_cond{
		pthread_mutex_t mtx;
		pthread_cond_t cond;
		int notifyd;
	};
#endif

CANDY_EXPORT void candy_thread_start(struct candy_thread*,candy_thread_fn fn,void *arg);
CANDY_EXPORT thread_id candy_thread_self();
CANDY_EXPORT thread_id candy_thread_id(struct candy_thread*);
CANDY_EXPORT void candy_thread_stop(struct candy_thread*);

CANDY_EXPORT void candy_mutex_init(struct candy_mutex*);
CANDY_EXPORT void candy_mutex_lock(struct candy_mutex*);
CANDY_EXPORT void candy_mutex_unlock(struct candy_mutex*);
CANDY_EXPORT void candy_mutex_destroy(struct candy_mutex*);

CANDY_EXPORT void candy_cond_init(struct candy_cond*);
CANDY_EXPORT void candy_cond_wait(struct candy_cond*);
CANDY_EXPORT void candy_cond_notify(struct candy_cond*);
CANDY_EXPORT void candy_cond_destroy(struct candy_cond*);

#endif
