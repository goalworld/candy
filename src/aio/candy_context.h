#ifndef CANDY_CONTEXT_H_
#define CANDY_CONTEXT_H_

#include "candy_worker_pool.h"
#include "candy_aio_pool.h"
#include "candy_timer_pool.h"
struct candy_aio;
struct candy_worker;

struct candy_context{
	struct candy_aio_pool aio_pool;
	struct candy_worker_pool worker_pool;
	struct candy_timer_pool timer_pool;
};

void candy_context_init(struct candy_context* self,int num_thread,int max_aio,int num_timer);
void candy_context_destroy(struct candy_context* self);
struct candy_aio* candy_context_get_aio(struct candy_context* self,int s);
int candy_context_create_aio(struct candy_context* self);
int candy_context_destroy_aio(struct candy_context* self,int s);
#define candy_context_wait(self) candy_worker_pool_wait(&(self)->worker_pool)
#define candy_context_stop(self) candy_worker_pool_stop(&(self)->worker_pool)
int candy_context_addtimer(struct candy_context* self,int timeout,int brepeat,candy_timerset_fn fn,void *arg);
#define candy_context_removetimer(self,id) candy_timer_pool_removetimer(&(self)->timer_pool,(id))
#endif


