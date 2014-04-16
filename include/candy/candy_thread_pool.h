#ifndef CANDY_THREAD_POOL_
#define CANDY_THREAD_POOL_

#include "./candy_queue.h"
#include "./candy_thread.h"
typedef void(*candy_thread_pool_fn)(void* arg);

struct candy_thread_pool{
	candy_mutex_t sync;
	candy_cond_t notify;
	struct candy_queue task_queue;
	candy_thread_t* threads;
	int bexit;
	int num_thread;
};

void candy_thread_pool_init(struct candy_thread_pool* tp,int num_thread);
void candy_thread_pool_execute(struct candy_thread_pool* tp,candy_thread_pool_fn fn,void* arg);
void candy_thread_pool_destroy(struct candy_thread_pool* tp);

#endif
