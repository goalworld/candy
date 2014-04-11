#ifndef CANDY_WORKER_H_
#define CANDY_WORKER_H_

#include "../base/candy_thread.h"
#include "../base/candy_queue.h"
#include "../base/candy_timer.h"
#include "candy_poller.h"

typedef void(*candy_worker_fn)(void* arg);

struct candy_worker{
	struct candy_poller poller;
	struct candy_timer timer;
	struct candy_thread thread;
	struct candy_queue queue;
	struct candy_mutex mutex;
	struct candy_poller_event event;
	int stop;
	thread_id id;
};

void candy_worker_init(struct candy_worker*);
void candy_worker_destroy(struct candy_worker*);
void candy_worker_add_event(struct candy_worker*,struct candy_poller_event*);
void candy_worker_remove_event(struct candy_worker*,struct candy_poller_event*);
void candy_worker_add_timer(struct candy_worker*,struct candy_timer_event*);
void candy_worker_remove_timer(struct candy_worker*,struct candy_timer_event*);
void candy_worker_execute(struct candy_worker*,candy_worker_fn fn,void *arg);
int candy_worker_in_loop(struct candy_worker*);
#endif
