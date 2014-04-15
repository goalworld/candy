#include "candy_thread_pool.h"
#include "candy_log.h"
#include <stdlib.h>

struct pool_task{
	candy_thread_pool_fn fn;
	void* arg;
};
static void candy_thread_pool_work(void* arg);
void candy_thread_pool_init(struct candy_thread_pool* tp,int num_thread){
	CANDY_CHECK(num_thread > 0);
	tp->bexit = 1;
	tp->num_thread = num_thread;
	candy_queue_init(&tp->task_queue,sizeof(struct pool_task));
	candy_cond_init(&tp->notify);
	candy_mutex_init(&tp->sync);
	tp->threads = (struct candy_thread*)malloc(sizeof(struct candy_thread)*num_thread);
	int i=0;
	for(i=0;i<num_thread;i++){
		candy_thread_start(&tp->threads[i],candy_thread_pool_work,tp);
	}

}
void candy_thread_pool_work(void* arg){
	struct candy_thread_pool* tp = (struct candy_thread_pool*)(arg);
	struct pool_task task;
	int rc;
	while(tp->bexit){
		candy_mutex_lock(&tp->sync);
		rc = candy_queue_pop(&tp->task_queue,&task);
		candy_mutex_unlock(&tp->sync);
		if(rc == 0){
			task.fn(task.arg);
		}else{
			candy_cond_wait(&tp->notify);
		}
	}
}
void candy_thread_pool_execute(struct candy_thread_pool* tp,candy_thread_pool_fn fn,void* arg){
	struct pool_task task;
	task.fn = fn;
	task.arg = arg;
	candy_mutex_lock(&tp->sync);
	candy_queue_push(&tp->task_queue,&task);
	candy_mutex_unlock(&tp->sync);
	candy_cond_notify(&tp->notify);
}
void candy_thread_pool_destroy(struct candy_thread_pool* tp){
	tp->bexit = 0;
	candy_cond_notify(&tp->notify);
	int i=0;
	for(i=0;i<tp->num_thread;i++){
		candy_thread_stop(&tp->threads[i]);
	}
	candy_cond_destroy(&tp->notify);
	candy_mutex_destroy(&tp->sync);
}
