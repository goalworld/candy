#include "./candy_aio_pool.h"
#include "./candy_aio.h"
#include "./candy_worker.h"
#include <stdlib.h>
#include <memory.h>
#include "../base/candy_log.h"
static void candy_close_and_free_aio(void* arg);
void candy_aio_pool_init(struct candy_aio_pool* self,int max_aio){
	int i;
	candy_mutex_init(&self->mtx);
	self->max_aio = max_aio;
	self->num_aio = 0;
	self->aio_map =  (struct candy_aio**)malloc(sizeof(void*)*self->max_aio);
	self->unused = (int*)malloc(sizeof(int)*self->max_aio);
	memset(self->aio_map, 0 , sizeof(void*)*self->max_aio);
	for(i=0;i<self->max_aio;i++){
		self->unused[i] = self->max_aio - i -1;
	}
}
void candy_aio_pool_destroy(struct candy_aio_pool* self){
	int i;
	for(i=0;i<self->max_aio;i++){
		struct candy_aio* aio = self->aio_map[i];
		if(aio){
			candy_aio_execute(aio,candy_close_and_free_aio,(void*)aio);
		}
	}
	free(self->aio_map);
	free(self->unused );
	candy_mutex_destroy(&self->mtx);
}
struct candy_aio* candy_aio_pool_get_aio(struct candy_aio_pool* self,int handle){
	candy_mutex_lock(&self->mtx);
	if(handle >= self->max_aio){
		candy_mutex_unlock(&self->mtx);
		return NULL;
	}
	struct candy_aio* aio = self->aio_map[handle];
	candy_mutex_unlock(&self->mtx);
	return aio;
}
struct candy_aio* candy_aio_pool_alloc_aio(struct candy_aio_pool* self,struct candy_worker* worker){
	candy_mutex_lock(&self->mtx);
	if(self->num_aio >= self->max_aio){
		CANDY_INFO("candy_context_alloc_aio is full %d",self->max_aio);
		candy_mutex_unlock(&self->mtx);
		return NULL;
	}
	int handle = self->unused[self->max_aio-self->num_aio-1];
	self->num_aio++;
	struct candy_aio* aio = (struct candy_aio*)malloc(sizeof(struct candy_aio));
	self->aio_map[handle] = aio;
	candy_aio_init(aio,self,handle,worker);
	candy_mutex_unlock(&self->mtx);
	return aio;
}
int candy_aio_pool_free(struct candy_aio_pool* self,int handle){
	candy_mutex_lock(&self->mtx);
	struct candy_aio* aio = self->aio_map[handle];
	if(!aio){
		candy_mutex_unlock(&self->mtx);
		return -CANDY_ENOTAIO;
	}
	self->num_aio--;
	self->unused[self->max_aio-self->num_aio-1] = handle;
	self->aio_map[handle] = NULL;
	candy_aio_execute(aio,candy_close_and_free_aio,(void*)aio);
	candy_mutex_unlock(&self->mtx);
	return 0;
}

void candy_close_and_free_aio(void* arg){
	struct candy_aio* aio = (struct candy_aio*)arg;
	CANDY_ERROR("candy_close_and_free_aio handle:%d",candy_aio_get_handle(aio));
	candy_aio_destroy(aio);
	free(aio);
}

