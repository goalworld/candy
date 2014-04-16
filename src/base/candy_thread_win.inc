
#include "candy/candy_log.h"
#include "candy_window.h"
#include <stdlib.h>
struct candy_thread{
    candy_thread_fn fn;
    void * arg;
    HANDLE handle;
};

struct candy_mutex{
    CRITICAL_SECTION cs;
};

struct candy_cond{
    HANDLE hevent;
};
static unsigned int __stdcall candy_thread_main_routine (void *arg){
    struct candy_thread *self = (struct candy_thread*) arg;
    self->fn (self->arg);
    return 0;
}
void candy_thread_start(candy_thread_t* t,candy_thread_fn fn,void *arg){
    candy_thread_t self = (candy_thread_t)malloc(sizeof( struct candy_thread));
	self->fn = fn;
	self->arg = arg;
	self->handle = (HANDLE) _beginthreadex (NULL, 0,
        candy_thread_main_routine, (void*) self, 0 , NULL);
   
	CANDY_CHECK (self->handle != NULL);
    *t = self;
}
int candy_thread_is_in(candy_thread_t self){
   return self->handle == GetCurrentThread() ? 0:-1;
}

void candy_thread_stop(candy_thread_t self){
	DWORD rc;
    BOOL brc;

    rc = WaitForSingleObject (self->handle, INFINITE);
    CANDY_CHECK (rc != WAIT_FAILED);
    brc = CloseHandle (self->handle);
    CANDY_CHECK (brc != 0);
    free(self);
}

void candy_mutex_init(candy_mutex_t* t){
    candy_mutex_t mtx = (struct candy_mutex*)malloc(sizeof( struct candy_mutex));
    *t = mtx;
	InitializeCriticalSection(&mtx->cs);
}
void candy_mutex_lock(candy_mutex_t mtx){
	EnterCriticalSection(&mtx->cs);
}
void candy_mutex_unlock(candy_mutex_t mtx){
	LeaveCriticalSection(&mtx->cs);
}
void candy_mutex_destroy(candy_mutex_t mtx){
	DeleteCriticalSection(&mtx->cs);
    free(mtx);
}

void candy_cond_init(candy_cond_t* t){
    candy_cond_t cond = (struct candy_cond*)malloc(sizeof( struct candy_cond));
    *t = cond;
    cond->hevent = CreateEvent(NULL,true,false,NULL);
}
void candy_cond_wait(struct candy_cond* cond){
    WaitForSingleObject(cond->hevent,INFINITE);
    ResetEvent(cond->hevent);
}
void candy_cond_notify(struct candy_cond* cond){
    SetEvent(cond->hevent);
}
void candy_cond_destroy(struct candy_cond* cond){
    CloseHandle(cond->hevent);
    free(cond);
}