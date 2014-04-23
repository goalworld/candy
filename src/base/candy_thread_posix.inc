
#include "candy/candy_log.h"
#include <pthread.h>
#include <stdlib.h>
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
static void* candy_thread_main_routine (void *arg){
    struct candy_thread *self;
    self = (struct candy_thread*) arg;
    self->fn(self->arg);
    return 0;
}
void candy_thread_start(candy_thread_t* t,candy_thread_fn fn,void *arg){
    candy_thread_t self = (candy_thread_t)malloc(sizeof( struct candy_thread));
	self->fn = fn;
	self->arg = arg;
	int rc = pthread_create(&self->pid,NULL,candy_thread_main_routine,self);
    CANDY_CHECK(rc == 0);
    *t = self;
}
int candy_thread_is_in(candy_thread_t self){
    return self->pid == pthread_self() ? 0 : -1;
}
void candy_thread_wait(candy_thread_t self){
    void* ret;
    int rc = pthread_join(self->pid,&ret);
    CANDY_CHECK(rc == 0);
}
void candy_thread_stop(candy_thread_t self){
	candy_thread_wait(self);
    free(self);
}

void candy_mutex_init(candy_mutex_t* t){
    candy_mutex_t mtx = (struct candy_mutex*)malloc(sizeof( struct candy_mutex));
    *t = mtx;
	int rc = pthread_mutex_init(&mtx->mtx,NULL);
    CANDY_CHECK(rc == 0);
}
void candy_mutex_lock(candy_mutex_t mtx){
	int rc = pthread_mutex_lock(&mtx->mtx);
    CANDY_CHECK(rc == 0);
}
void candy_mutex_unlock(candy_mutex_t mtx){
	int rc = pthread_mutex_unlock(&mtx->mtx);
    CANDY_CHECK(rc == 0);
}
void candy_mutex_destroy(candy_mutex_t mtx){
	int rc = pthread_mutex_destroy(&mtx->mtx);
    CANDY_CHECK(rc == 0);
    free(mtx);
}


void candy_cond_init(candy_cond_t* t){
    candy_cond_t cond = (struct candy_cond*)malloc(sizeof( struct candy_cond));
    *t = cond;
    CANDY_CHECK(pthread_cond_init(&cond->cond,NULL) == 0);
    CANDY_CHECK(pthread_mutex_init(&cond->mtx,NULL) == 0);
    
}
void candy_cond_wait(candy_cond_t cond){
    CANDY_CHECK(pthread_mutex_lock(&cond->mtx) == 0);
    if(!cond->notifyd){
        pthread_cond_wait(&cond->cond,&cond->mtx);
    }
    CANDY_CHECK(pthread_mutex_unlock(&cond->mtx) == 0);
}
void candy_cond_notify(candy_cond_t cond){
    CANDY_CHECK(pthread_mutex_lock(&cond->mtx)== 0);
    cond->notifyd = 1;
    pthread_cond_broadcast(&cond->cond);
    CANDY_CHECK(pthread_mutex_unlock(&cond->mtx)== 0);
}
void candy_cond_destroy(candy_cond_t cond){
    pthread_cond_destroy(&cond->cond);
    pthread_mutex_destroy(&cond->mtx);
    free(cond);
}