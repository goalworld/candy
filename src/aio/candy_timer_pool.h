#ifndef CANDY_TIMER_POOL_H_
#define CANDY_TIMER_POOL_H_

#include "candy/candy_thread.h"
#include "candy_timerset.h"
#define CANDY_AIO_POOL_INVILD_HANDLE -1
struct candy_worker;
struct candy_timer{
	struct candy_worker* owner;
	struct candy_timerset_event event;
};

struct candy_timer_pool{
	candy_mutex_t mtx;
	struct candy_timer** timer_map;
	int* unused;
	int max;
	int num;
};
void candy_timer_pool_init(struct candy_timer_pool* self,int max);
void candy_timer_pool_destroy(struct candy_timer_pool* self);
int candy_timer_pool_addtimer(struct candy_timer_pool* self,
								struct candy_worker* worker,
								int timeout,
								int brepeat,
								candy_timerset_fn fn,
								void *arg);
int candy_timer_pool_removetimer(struct candy_timer_pool* self,int id);

#endif
