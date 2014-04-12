#include "candy_worker.h"
#include "../base/candy_log.h"

struct candy_task{
	candy_worker_fn fn;
	void *arg;
};
static void candy_worker_run_queue(void *arg);
static void candy_worker_run(void *arg);
static void candy_worker_run(void *arg){
	struct candy_worker* self = (struct candy_worker*)arg;
	while(!self->stop){
		int min = candy_timer_execute(&self->timer);
		candy_poller_poll(&self->poller,min);
		candy_worker_run_queue(arg);
	}
}
static void candy_worker_run_queue(void *arg){
	struct candy_worker* self = (struct candy_worker*)arg;
	struct candy_task task;
	int rc;
	while(1){
		candy_mutex_lock(&self->mutex);
		rc = candy_queue_pop(&self->queue,&task);
		candy_mutex_unlock(&self->mutex);
		if(rc != 0) break;
		task.fn(task.arg);
	}
}
void candy_worker_init(struct candy_worker* self,struct candy_worker_pool*owner){
	self->stop = 0;
	self->owner = owner;
	candy_poller_init(&self->poller);
	candy_timer_init(&self->timer);
	candy_mutex_init(&self->mutex);
	//candy_poller_event_init(&self->event,&self->poller,0,candy_worker_run_queue,self);
	//candy_poller_add(&self->poller,&self->event);
	//candy_poller_event_set_in(&self->event);
	candy_queue_init(&self->queue,sizeof(struct candy_task));
	candy_thread_start(&self->thread,candy_worker_run,(void*)self);
}
void candy_worker_destroy(struct candy_worker* self){
	self->stop = 1;
	//candy_poller_remove(&self->poller,&self->event);
	candy_poller_destroy(&self->poller);
	candy_thread_stop(&self->thread);
	candy_mutex_destroy(&self->mutex);
}

void candy_worker_add_event(struct candy_worker* self,struct candy_poller_event* ev){
	candy_poller_add(&self->poller,ev);
}
void candy_worker_remove_event(struct candy_worker* self,struct candy_poller_event* ev){
	candy_poller_remove(&self->poller,ev);
}
void candy_worker_add_timer(struct candy_worker* self,struct candy_timer_event* ev){
	candy_timer_add(&self->timer,ev);
}
void candy_worker_remove_timer(struct candy_worker* self,struct candy_timer_event* ev){
	candy_timer_remove(&self->timer,ev);
}
void candy_worker_execute(struct candy_worker* self,candy_worker_fn fn,void *arg){
	if(candy_thread_self() == candy_thread_id(&self->thread)){
		fn(arg);
	}else{
		candy_mutex_lock(&self->mutex);
		struct candy_task task;
		task.fn = fn;
		task.arg = arg;
		candy_queue_push(&self->queue,&task);
		candy_mutex_unlock(&self->mutex);
	}
}

int candy_worker_in_loop(struct candy_worker* self){
	return (candy_thread_self() == candy_thread_id(&self->thread))?0:-1;
}
