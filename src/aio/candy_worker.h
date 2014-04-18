#ifndef CANDY_WORKER_H_
#define CANDY_WORKER_H_

#include "candy/candy_thread.h"
#include "candy/candy_queue.h"
#include "candy_timerset.h"
#include "candy_poller.h"

typedef void(*candy_worker_fn)(void* arg);

struct candy_worker{
	struct candy_worker_pool* owner;
	struct candy_poller poller;
	struct candy_timerset timer;
	candy_thread_t thread;
	struct candy_queue queue;
	candy_mutex_t mutex;
	struct candy_poller_event event;
	int stop;
};
#define candy_worker_owner(ptr) (ptr)->owner
void candy_worker_init(struct candy_worker*,struct candy_worker_pool*);
void candy_worker_destroy(struct candy_worker*);
void candy_worker_add_event(struct candy_worker*,struct candy_poller_event*);
void candy_worker_remove_event(struct candy_worker*,struct candy_poller_event*);
void candy_worker_add_timer(struct candy_worker*,struct candy_timerset_event*);
void candy_worker_remove_timer(struct candy_worker*,struct candy_timerset_event*);
void candy_worker_execute(struct candy_worker*,candy_worker_fn fn,void *arg);
void candy_worker_queue_execute(struct candy_worker*,candy_worker_fn fn,void *arg);
int candy_worker_in_loop(struct candy_worker*);
void candy_worker_wait(struct candy_worker* worker);
void candy_worker_stop(struct candy_worker* worker);
#endif
