
#include <pthread.h>
#include "candy_log.h"

static void* candy_thread_main_routine (void *arg){
    struct candy_thread *self;
    self = (struct candy_thread*) arg;
    self->fn(self->arg);
    return 0;
}
void candy_thread_start(struct candy_thread* self,candy_thread_fn fn,void *arg){
	self->fn = fn;
	self->arg = arg;
	int rc = pthread_create(&self->pid,NULL,candy_thread_main_routine,self);
    CANDY_CHECK(rc == 0);
}
thread_id candy_thread_self(){
	return pthread_self();
}
thread_id candy_thread_id(struct candy_thread* self){
    return self->pid;
}
void candy_thread_stop(struct candy_thread* self){
	void* ret; 
    int rc = pthread_join(self->pid,&ret);
    CANDY_CHECK(rc == 0);
}

void candy_mutex_init(struct candy_mutex* mtx){
	int rc = pthread_mutex_init(&mtx->mtx,NULL);
    CANDY_CHECK(rc == 0);
}
void candy_mutex_lock(struct candy_mutex* mtx){
	int rc = pthread_mutex_lock(&mtx->mtx);
    CANDY_CHECK(rc == 0);
}
void candy_mutex_unlock(struct candy_mutex* mtx){
	int rc = pthread_mutex_unlock(&mtx->mtx);
    CANDY_CHECK(rc == 0);
}
void candy_mutex_destroy(struct candy_mutex* mtx){
	int rc = pthread_mutex_destroy(&mtx->mtx);
    CANDY_CHECK(rc == 0);
}


void candy_cond_init(struct candy_cond* cond){
    CANDY_CHECK(pthread_cond_init(&cond->cond,NULL) == 0);
    CANDY_CHECK(pthread_mutex_init(&cond->mtx,NULL) == 0);
}
void candy_cond_wait(struct candy_cond* cond){
    CANDY_CHECK(pthread_mutex_lock(&cond->mtx) == 0);
    if(!this->notifyd){
        pthread_cond_wait(&cond->cond,&cond->mtx);
    }
    CANDY_CHECK(pthread_mutex_unlock(&cond->mtx) == 0);
}
void candy_cond_notify(struct candy_cond* cond){
    CANDY_CHECK(pthread_mutex_lock(&cond->mtx)== 0);
    cond->notifyd = 1;
    pthread_cond_broadcast(&cond->cond);
    CANDY_CHECK(pthread_mutex_unlock(&cond->mtx)== 0);
}
void candy_cond_destroy(struct candy_cond* cond){
    pthread_cond_destroy(&cond->cond);
    pthread_mutex_destroy(&cond->mtx);
}