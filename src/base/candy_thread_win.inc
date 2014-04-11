
#include "candy_log.h"

static unsigned int __stdcall candy_thread_main_routine (void *arg){
    struct candy_thread *self = (struct candy_thread*) arg;
    self->id = candy_thread_self();
    self->fn (self->arg);
    return 0;
}
void candy_thread_start(struct candy_thread* self,candy_thread_fn fn,void *arg){
	self->fn = fn;
	self->arg = arg;
	self->handle = (HANDLE) _beginthreadex (NULL, 0,
        candy_thread_main_routine, (void*) self, 0 , NULL);
   
	CANDY_CHECK (self->handle != NULL);
}
thread_id candy_thread_self(){
	return GetCurrentThreadId();
}
thread_id candy_thread_id(struct candy_thread* self){
    return self->id;
}
void candy_thread_stop(struct candy_thread* self){
	DWORD rc;
    BOOL brc;

    rc = WaitForSingleObject (self->handle, INFINITE);
    CANDY_CHECK (rc != WAIT_FAILED);
    brc = CloseHandle (self->handle);
    CANDY_CHECK (brc != 0);
}

void candy_mutex_init(struct candy_mutex* mtx){
	InitializeCriticalSection(&mtx->cs);
}
void candy_mutex_lock(struct candy_mutex* mtx){
	EnterCriticalSection(&mtx->cs);
}
void candy_mutex_unlock(struct candy_mutex* mtx){
	LeaveCriticalSection(&mtx->cs);
}
void candy_mutex_destroy(struct candy_mutex* mtx){
	DeleteCriticalSection(&mtx->cs);
}
