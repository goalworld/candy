#include "candy_timer_pool.h"
#include "candy_worker.h"
#include "candy/candy_log.h"
#include <stdlib.h>

static void _do_regist(void*arg){
	struct candy_timer* timer = (struct candy_timer* )arg;
	candy_worker_add_timer(timer->owner,&timer->event);
}
static void _do_destroy(void*arg){
	struct candy_timer* timer = (struct candy_timer* )arg;
	candy_worker_remove_timer(timer->owner,&timer->event);
	free(timer);
}
void candy_timer_pool_init(struct candy_timer_pool* self,int max){
	int i;
	candy_mutex_init(&self->mtx);
	self->max = max;
	self->num = 0;
	self->timer_map =  (struct candy_timer**)malloc(sizeof(void*)*self->max);
	self->unused = (int*)malloc(sizeof(int)*self->max);
	memset(self->timer_map, 0 , sizeof(void*)*self->max);
	for(i=0;i<self->max;i++){
		self->unused[i] = self->max - i -1;
	}
}
void candy_timer_pool_destroy(struct candy_timer_pool* self){
	int i;
	for(i=0;i<self->max;i++){
		struct candy_timer* timer = self->timer_map[i];
		if(timer){
			candy_worker_execute(timer->owner,_do_destroy,timer);
		}
	}
	free(self->timer_map);
	free(self->unused );
	candy_mutex_destroy(self->mtx);
}
int candy_timer_pool_addtimer(struct candy_timer_pool* self,
								struct candy_worker* worker,
								int timeout,
								int brepeat,
								candy_timerset_fn fn,
								void *arg)
{
	candy_mutex_lock(self->mtx);
	if(self->num >= self->max){
		CANDY_INFO("candy_context_alloc_timer is full %d",self->max);
		candy_mutex_unlock(self->mtx);
		return -1;
	}
	int handle = self->unused[self->max-self->num-1];
	self->num++;
	struct candy_timer* timer = (struct candy_timer*)malloc(sizeof(struct candy_timer));
	self->timer_map[handle] = timer;
	timer->owner = worker;
	candy_timerset_event_init(&timer->event,timeout,brepeat,fn,arg,handle);
	candy_worker_execute(worker,_do_regist,timer);
	candy_mutex_unlock(self->mtx);
	return handle;
}
int candy_timer_pool_removetimer(struct candy_timer_pool* self,int handle){
	candy_mutex_lock(self->mtx);
	struct candy_timer* timer = self->timer_map[handle];
	if(!timer){
		candy_mutex_unlock(self->mtx);
		return -1;
	}
	self->num--;
	self->unused[self->max-self->num-1] = handle;
	self->timer_map[handle] = NULL;
	candy_timerset_event_remove(&timer->event);
	candy_worker_execute(timer->owner,_do_destroy,timer);
	candy_mutex_unlock(self->mtx);
	return 0;
}
