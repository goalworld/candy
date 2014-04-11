#include "../candy.h"
#include "candy_context.h"
#include "../base/candy_log.h"
#include "../aio/candy_aio.h"
#include "../aio/candy_worker.h"
#include "../base/candy_window.h"
#include "../base/candy_glock.h"

#include <stdlib.h>
#include <memory.h>

struct candy_context{
	int inited;
	struct candy_aio** aio_map;
	int* unused;
	int max_aio;
	int num_aio;
	struct candy_worker *workers;
	int num_work;
	int cur_work;
	struct candy_mutex mtx;
};

static struct candy_context self = {0};
static void candy_close_and_free_aio(void*arg);
int candy_context_inited(){
	return self.inited?0:1;
}
int 
candy_context_init(int num_thread,int max_aio){
	candy_glock_lock();
	if(self.inited) {
		candy_glock_unlock();
		return -1;
	}
	CANDY_INFO("num_thread:%d,max_aio%d",num_thread,max_aio);
	int i;
	self.inited = 1;
	self.max_aio = max_aio;
	self.num_aio = 0;
	self.aio_map =  (struct candy_aio**)malloc(sizeof(void*)*self.max_aio);
	self.unused = (int*)malloc(sizeof(int)*self.max_aio);
	memset(self.aio_map, 0 , sizeof(void*)*self.max_aio);
	for(i=0;i<self.max_aio;i++){
		self.unused[i] = self.max_aio - i -1;
	}
	#if defined CANDY_HAVE_WINDOWS
		WSADATA wsa={0};	
		WSAStartup(MAKEWORD(2,2),&wsa);
	#endif
	self.num_work = num_thread;
	self.cur_work = 0;
	self.workers = (struct candy_worker*)malloc(sizeof(struct candy_worker)*self.num_work);
	for(i=0;i<self.num_work;i++){
		candy_worker_init(&self.workers[i]);
	}
	candy_mutex_init(&self.mtx);
	candy_glock_unlock();
	return 0;
}
int 
candy_context_destroy(){
	candy_glock_lock();
	if(!self.inited) {
		candy_glock_unlock();
		return -1;
	}
	int i;

	self.inited = 0;
	for(i=0;i<self.max_aio;i++){
		candy_worker_destroy(&self.workers[i]);
	}
	free(self.workers );
	for(i=0;i<self.max_aio;i++){
		struct candy_aio* aio = self.aio_map[i];
		if(aio){
			candy_aio_close(aio);
		}
	}
	free(self.aio_map);
	free(self.unused );
	#if defined CANDY_HAVE_WINDOWS
		WSACleanup();
	#endif
	candy_glock_unlock();
	return 0;
}
struct candy_worker* 
candy_context_next_worker(){
	struct candy_worker* worker = &self.workers[self.cur_work];
	self.cur_work = (self.cur_work+1)%self.num_work;
	return worker;
}
struct candy_aio* candy_context_alloc_aio(){
	candy_glock_lock();
	if(self.num_aio >= self.max_aio){
		CANDY_INFO("candy_context_alloc_aio is full %d",self.max_aio);
		candy_glock_unlock();
		return NULL;
	}
	int s = self.unused[self.max_aio-self.num_aio-1];
	self.num_aio++;
	struct candy_aio* aio = (struct candy_aio*)malloc(sizeof(struct candy_aio));
	self.aio_map[s] = aio;
	struct candy_worker* worker = candy_context_next_worker(); 
	candy_aio_init(aio,s,worker);
	candy_glock_unlock();
	return aio;
}
int 
candy_context_create_aio(){
	struct candy_aio* aio = candy_context_alloc_aio();
	if(!aio){
		return -1;
	}
	return candy_aio_get_handle(aio);
}
int 
candy_context_destroy_aio(int s){
	candy_mutex_lock(&self.mtx);
	struct candy_aio* aio = self.aio_map[s];
	if(!aio){
		candy_mutex_unlock(&self.mtx);
		return -CANDY_ENOTAIO;
	}
	self.num_aio--;
	self.unused[self.max_aio-self.num_aio-1] = s;
	self.aio_map[s] = NULL;
	candy_worker_execute(candy_aio_get_worker(aio),candy_close_and_free_aio,(void*)aio);
	candy_mutex_unlock(&self.mtx);
	return 0;
}
void candy_close_and_free_aio(void* arg){

	struct candy_aio* aio = (struct candy_aio*)arg;
	CANDY_ERROR("candy_close_and_free_aio handle:%d",candy_aio_get_handle(aio));
	candy_aio_close(aio);
	free(aio);
}

struct candy_aio* 
candy_context_get_aio(int s){
	if(!self.inited){
		return NULL;
	}
	candy_mutex_lock(&self.mtx);
	if(s >= self.max_aio){
		candy_mutex_unlock(&self.mtx);
		return NULL;
	}
	struct candy_aio* aio = self.aio_map[s];
	if(!aio){
		candy_mutex_unlock(&self.mtx);
		return NULL; 
	}
	candy_mutex_unlock(&self.mtx);
	return aio;
}





